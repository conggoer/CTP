/**
  ********************************  STM32F103RCT6 *********************************
  *********************************  FreeRTOS  *********************************
  * @文件名     ： app_task.c
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @系统版本   ： V9.0.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： 应用程序任务源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-09-24 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "app_task.h"
#include "queue.h"
/* 使用函数声明---------------------------------------------------------------*/
void get_getai_height2(void);
void get_getai_height1(void);
void PIDRun(void);
/*变量定义-------------------------------------------------------------------*/

u16 count,duty_value = 2000;
u16 getai_mm, fangxing_mm, zuowu_mm;
u8 cantxbuf[8];
u8 canrxbuf[8];
u8 sendbuf[8];
int flagUp = 0;   //为1手动控制割台，为0没有手动
int flagDown = 0;
int flagRun = 0;  //自动部分程序运行标志位
float getai_height, getai_height1, getai_height2, getai_height3;
uint32_t voltage1_value;         //电压的转换值adc值
float voltage1;
u8 Can_Receive_flag;
extern u8 auto_control,reset_height;
/*队列------------------------------------------------------------------*/
extern xQueueHandle  HQueue; //队列句柄



/************************************************
函数名称 ： Task_Measure
功    能 ： 距离测量
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： acong
*************************************************/
void Task_Measure(void *pvParameters)
{
  unsigned int SendHeight = 0;
  for(;;)
  {
    //超声波距离值读取
    get_getai_height2();
    printf("%f\r\n",getai_height2);
    getai_height = getai_height2;                   //主要获得getai_height这个变量
    getai_mm =  (unsigned int) (getai_height * 10);
    SendHeight = getai_mm;
    //角度传感器电压
    voltage1_value = Get_Adc_Average(ADC_Channel_1,10);                       //检测电压, adc的值
    voltage1 = (float)voltage1_value*(3.3/4096);   //检测电压值
    voltage1 = voltage1 * 15.1 / 10;
    printf("角度传感器输出电压=%fv\r\n",voltage1);
    printf("task1\r\n");
    xQueueSend(HQueue,(void*)&SendHeight,0);
    vTaskDelay(2000);
  }
}

/************************************************
函数名称 ： Task_Control
功    能 ： PID控制功能
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： acong
*************************************************/
void Task_Control(void *pvParameters)
{
  for(;;)
  {
    if(flagUp != 0)
    {
      TIM_SetCompare2(TIM3,3600); //割台升高
      TIM_SetCompare1(TIM3,0);
    }
    else if(flagUp == 0 && flagRun == 0)  //flagRun为1不能清零否则会一直为零
    {
      TIM_SetCompare2(TIM3,0);
    }
    if(flagDown != 0 )
    {
      TIM_SetCompare1 (TIM3,3600);  //割台降低
      TIM_SetCompare2 (TIM3,0);
    }
    else if(flagDown == 0 && flagRun == 0)
    {
      TIM_SetCompare1(TIM3,0);
    }
    flagRun = 0; //每次运行前清零
    printf("flagUp = %d\r\n",flagUp);
    printf("flagDown = %d\r\n",flagDown);
    TIMDelay_Nms(200);
    if(flagUp == 0 && flagDown == 0)
    {
      flagRun = 1;
      PIDRun();
      printf("task2\r\n");
    }
    vTaskDelay(1000);
  }
}

/************************************************
函数名称 ： Task_Communication
功    能 ： 通信任务，用于CAN总线的数据收发
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： acong
*************************************************/
void Task_Communication(void *pvParameters)
{
  unsigned int ReceiveHeight = 0;
  for(;;)
  {
    //接收部分：获取指令
    Can_Receive_flag = Can_Receive_Msg();		//获得两个参数的值：reset_height、auto_control
    if(Can_Receive_flag)
      printf("可以接收,auto_control = %d\r\n",auto_control); 
    //发送部分：发送仿形高度、割台高度和作物高度值
   // sendbuf[0]=fangxing_mm & 0x00ff;  //仿形高度L
    //sendbuf[1]=fangxing_mm >> 8;     //仿形高度H
    //sendbuf[2]=getai_mm & 0x00ff;   //割台高度L
   // sendbuf[3]=getai_mm>>8;         //割台高度H
    xQueueReceive(HQueue,&ReceiveHeight,100/portTICK_RATE_MS);
    sendbuf[2]=ReceiveHeight & 0x00ff;   //割台高度L
    sendbuf[3]=ReceiveHeight>>8;         //割台高度H
   // sendbuf[4]=zuowu_mm & 0x00ff;   //作物高度L
   // sendbuf[5]=zuowu_mm >> 8;
    Can_Send_Msg(sendbuf,8);
    printf("task3\r\n");
    vTaskDelay(500);
  }
}

/************************************************
函数名称 ： Task_Inquiry 
功    能 ： 引脚电平查询任务，用于监视PB4、PB5状态来判断手动控制割台升降
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： acong
*************************************************/
void Task_Inquiry(void *pvParameters)
{
  flagUp = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4); //读取IO电平状态
  flagDown = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
}






