/**
  ********************************  STM32F103RCT6  *********************************
  *********************************  FreeRTOS  *********************************
  * @�ļ���     �� main.c
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @ϵͳ�汾   �� V9.0.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� ������ - CTP�������
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-09-24 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "bsp.h"
#include "task.h"
#include "app_task.h"
#include "timer.h"
#include "queue.h"
/* �궨�� --------------------------------------------------------------------*/
#define TASK1_STACK_SIZE          1280
#define TASK1_PRIORITY            3

#define TASK2_STACK_SIZE          128
#define TASK2_PRIORITY            2

#define TASK3_STACK_SIZE          128
#define TASK3_PRIORITY            1

#define TASK4_STACK_SIZE          128
#define TASK4_PRIORITY            1
xQueueHandle HQueue; //���о��


/************************************************
�������� �� SysInit
��    �� �� ϵͳ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
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
�������� �� CongfigInit
��    �� �� �������˿ڡ����ܳ�ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/

void ConfigInit(void)
{
 // u8 a,flag;
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,16,CAN_Mode_Normal);//CAN��ʼ����ͨģʽ,������250Kbps
  Tim2_Init();
  TIM3_PWM_Config(7199,49);  //PWMƵ��=72000/7200/50=200hz
  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
  GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12);
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
/************************************************
�������� �� AppTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void AppTaskCreate(void)
{
  xTaskCreate(Task_Measure, "Task1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
  xTaskCreate(Task_Control, "Task2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);
  xTaskCreate(Task_Communication, "Task3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);
  xTaskCreate(Task_Inquiry, "Task4", TASK3_STACK_SIZE, NULL, TASK4_PRIORITY, NULL);
 
}

/************************************************
�������� �� main
��    �� �� ���������
��    �� �� ��
�� �� ֵ �� int
��    �� �� acong
*************************************************/
int main(void)
{
  
  /* 1����ʼ�� */
  SysInit();
  ConfigInit();
  /* 2���������� */
 HQueue = xQueueCreate(5,sizeof(unsigned int));
 AppTaskCreate();
  //printf("test");
  /* 3���������� */
 vTaskStartScheduler();

  return 0;
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
