/**
  ********************************  STM32F103RCT6 *********************************
  *********************************  FreeRTOS  *********************************
  * @�ļ���     �� app_task.c
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @ϵͳ�汾   �� V9.0.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� Ӧ�ó�������Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-09-24 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "app_task.h"
#include "queue.h"
/* ʹ�ú�������---------------------------------------------------------------*/
void get_getai_height2(void);
void get_getai_height1(void);
void PIDRun(void);
/*��������-------------------------------------------------------------------*/

u16 count,duty_value = 2000;
u16 getai_mm, fangxing_mm, zuowu_mm;
u8 cantxbuf[8];
u8 canrxbuf[8];
u8 sendbuf[8];
int flagUp = 0;   //Ϊ1�ֶ����Ƹ�̨��Ϊ0û���ֶ�
int flagDown = 0;
int flagRun = 0;  //�Զ����ֳ������б�־λ
float getai_height, getai_height1, getai_height2, getai_height3;
uint32_t voltage1_value;         //��ѹ��ת��ֵadcֵ
float voltage1;
u8 Can_Receive_flag;
extern u8 auto_control,reset_height;
/*����------------------------------------------------------------------*/
extern xQueueHandle  HQueue; //���о��



/************************************************
�������� �� Task_Measure
��    �� �� �������
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void Task_Measure(void *pvParameters)
{
  unsigned int SendHeight = 0;
  for(;;)
  {
    //����������ֵ��ȡ
    get_getai_height2();
    printf("%f\r\n",getai_height2);
    getai_height = getai_height2;                   //��Ҫ���getai_height�������
    getai_mm =  (unsigned int) (getai_height * 10);
    SendHeight = getai_mm;
    //�Ƕȴ�������ѹ
    voltage1_value = Get_Adc_Average(ADC_Channel_1,10);                       //����ѹ, adc��ֵ
    voltage1 = (float)voltage1_value*(3.3/4096);   //����ѹֵ
    voltage1 = voltage1 * 15.1 / 10;
    printf("�Ƕȴ����������ѹ=%fv\r\n",voltage1);
    printf("task1\r\n");
    xQueueSend(HQueue,(void*)&SendHeight,0);
    vTaskDelay(2000);
  }
}

/************************************************
�������� �� Task_Control
��    �� �� PID���ƹ���
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void Task_Control(void *pvParameters)
{
  for(;;)
  {
    if(flagUp != 0)
    {
      TIM_SetCompare2(TIM3,3600); //��̨����
      TIM_SetCompare1(TIM3,0);
    }
    else if(flagUp == 0 && flagRun == 0)  //flagRunΪ1������������һֱΪ��
    {
      TIM_SetCompare2(TIM3,0);
    }
    if(flagDown != 0 )
    {
      TIM_SetCompare1 (TIM3,3600);  //��̨����
      TIM_SetCompare2 (TIM3,0);
    }
    else if(flagDown == 0 && flagRun == 0)
    {
      TIM_SetCompare1(TIM3,0);
    }
    flagRun = 0; //ÿ������ǰ����
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
�������� �� Task_Communication
��    �� �� ͨ����������CAN���ߵ������շ�
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void Task_Communication(void *pvParameters)
{
  unsigned int ReceiveHeight = 0;
  for(;;)
  {
    //���ղ��֣���ȡָ��
    Can_Receive_flag = Can_Receive_Msg();		//�������������ֵ��reset_height��auto_control
    if(Can_Receive_flag)
      printf("���Խ���,auto_control = %d\r\n",auto_control); 
    //���Ͳ��֣����ͷ��θ߶ȡ���̨�߶Ⱥ�����߶�ֵ
   // sendbuf[0]=fangxing_mm & 0x00ff;  //���θ߶�L
    //sendbuf[1]=fangxing_mm >> 8;     //���θ߶�H
    //sendbuf[2]=getai_mm & 0x00ff;   //��̨�߶�L
   // sendbuf[3]=getai_mm>>8;         //��̨�߶�H
    xQueueReceive(HQueue,&ReceiveHeight,100/portTICK_RATE_MS);
    sendbuf[2]=ReceiveHeight & 0x00ff;   //��̨�߶�L
    sendbuf[3]=ReceiveHeight>>8;         //��̨�߶�H
   // sendbuf[4]=zuowu_mm & 0x00ff;   //����߶�L
   // sendbuf[5]=zuowu_mm >> 8;
    Can_Send_Msg(sendbuf,8);
    printf("task3\r\n");
    vTaskDelay(500);
  }
}

/************************************************
�������� �� Task_Inquiry 
��    �� �� ���ŵ�ƽ��ѯ�������ڼ���PB4��PB5״̬���ж��ֶ����Ƹ�̨����
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void Task_Inquiry(void *pvParameters)
{
  flagUp = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4); //��ȡIO��ƽ״̬
  flagDown = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
}






