/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� bsp.c
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� BSP�弶֧�ְ�Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-09-24 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
�������� �� RCC_Configuration
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void RCC_Configuration(void)
{
  /* ʹ��APB2ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                           RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF  | 
                             RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE); 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ����APB1
}

/************************************************
�������� �� GPIO_Basic_Configuration
��    �� �� �������������������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //������
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;//TX   1,2,3,4,5,6,7
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;//RX
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12;    		//	0,8
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          //ѡ������ģʽΪ����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //���Ƶ�����50Hz
  GPIO_Init(GPIOB,&GPIO_InitStructure);

}

/************************************************
�������� �� NVIC_Configuration
��    �� �� NVIC����
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ���ȼ����� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* �����ж� */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

/************************************************
�������� �� BSP_Initializes
��    �� �� �ײ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void BSP_Initializes(void)
{
  RCC_Configuration();                                               //ʱ������
  GPIO_Basic_Configuration();                                        //IO����

  NVIC_Configuration();                                              //�ж�����
}
