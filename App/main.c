/**
  ********************************  STM32F103RCT6  *********************************
  *********************************  FreeRTOS  *********************************
  * @文件名     ： main.c
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @系统版本   ： V9.0.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： 主函数 - CTP软件工程
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-09-24 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "bsp.h"
#include "task.h"
#include "app_task.h"
#include "timer.h"
#include "queue.h"
/* 宏定义 --------------------------------------------------------------------*/
#define TASK1_STACK_SIZE          1280
#define TASK1_PRIORITY            3

#define TASK2_STACK_SIZE          128
#define TASK2_PRIORITY            2

#define TASK3_STACK_SIZE          128
#define TASK3_PRIORITY            1

#define TASK4_STACK_SIZE          128
#define TASK4_PRIORITY            1
xQueueHandle HQueue; //队列句柄


/************************************************
函数名称 ： SysInit
功    能 ： 系统初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void SysInit(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  BSP_Initializes();
  Adc_Init();	
  TIMER_Initializes();
  USART_Initializes();
}
/************************************************
函数名称 ： CongfigInit
功    能 ： 变量、端口、功能初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/

void ConfigInit(void)
{
 // u8 a,flag;
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,16,CAN_Mode_Normal);//CAN初始化普通模式,波特率250Kbps
  Tim2_Init();
  TIM3_PWM_Config(7199,49);  //PWM频率=72000/7200/50=200hz
  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
  GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12);
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
/************************************************
函数名称 ： AppTaskCreate
功    能 ： 创建任务
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void AppTaskCreate(void)
{
  xTaskCreate(Task_Measure, "Task1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
  xTaskCreate(Task_Control, "Task2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);
  xTaskCreate(Task_Communication, "Task3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);
  xTaskCreate(Task_Inquiry, "Task4", TASK3_STACK_SIZE, NULL, TASK4_PRIORITY, NULL);
 
}

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ： acong
*************************************************/
int main(void)
{
  
  /* 1、初始化 */
  SysInit();
  ConfigInit();
  /* 2、创建任务 */
 HQueue = xQueueCreate(5,sizeof(unsigned int));
 AppTaskCreate();
  //printf("test");
  /* 3、开启任务 */
 vTaskStartScheduler();

  return 0;
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
