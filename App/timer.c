/**
  ********************************  STM32F103RCT6  *********************************
  * @ÎÄ¼şÃû     £º timer.c
  * @×÷Õß       £º acong
  * @¿â°æ±¾     £º V3.5.0
  * @ÎÄ¼ş°æ±¾   £º V1.0.0
  * @ÈÕÆÚ       £º 2017Äê09ÔÂ24ÈÕ
  * @ÕªÒª       £º ¶¨Ê±Æ÷Ô´ÎÄ¼ş
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ¸üĞÂÈÕÖ¾:
  2017Äê09ÔÂ24ÈÕ V1.0.0:³õÊ¼°æ±¾
  ----------------------------------------------------------------------------*/
/* °üº¬µÄÍ·ÎÄ¼ş --------------------------------------------------------------*/
#include "timer.h"

extern unsigned char isJam;
/************************************************
º¯ÊıÃû³Æ £º TIMER_Initializes
¹¦    ÄÜ £º TIMER³õÊ¼»¯
²Î    Êı £º ÎŞ
·µ »Ø Öµ £º ÎŞ
×÷    Õß £º acong
*************************************************/
void TIMER_Initializes(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* RCCÊ±ÖÓÅäÖÃ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* TIM4Ê±»ùµ¥ÔªÅäÖÃ */
  TIM_TimeBaseStructure.TIM_Prescaler = TIM4_PRESCALER_VALUE;        //Ô¤·ÖÆµÖµ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊıÄ£Ê½
  TIM_TimeBaseStructure.TIM_Period = TIM4_PERIOD_TIMING;             //¶¨Ê±ÖÜÆÚ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //Ê±ÖÓ·ÖÆµÒò×Ó
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Ê¹ÄÜÔ¤·ÖÆµÖµ */
  TIM_ARRPreloadConfig(TIM4, ENABLE);                                //Ê¹ÄÜÖØÔØÖµ
}


void Tim2_Init(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

   TIM_DeInit(TIM2);//¸´Î»TIM2¶¨Ê±Æ÷
   TIM_TimeBaseStructure.TIM_Period = 49999;             //¶¨Ê±Ê±¼ä50ms,¼ÆËã¹«Ê½£º¶¨Ê±Ê±¼ä£¨us£© = £¨£¨1+TIM_Prescaler£©/72M£©*(1+TIM_Period)
   TIM_TimeBaseStructure.TIM_Prescaler = 71; 
   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
     
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);
     
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/************************************************
º¯ÊıÃû³Æ £º TIMDelay_N10us
¹¦    ÄÜ £º ¶¨Ê±Æ÷ÑÓÊ±N¸ö10us
²Î    Êı £º Times --- NÖµ
·µ »Ø Öµ £º ÎŞ
×÷    Õß £º acong
*************************************************/
void TIMDelay_N10us(uint16_t Times)
{
  TIM_Cmd(TIM4, ENABLE);                                             //Æô¶¯¶¨Ê±Æ÷
  while(Times--)
  {
    while(RESET == TIM_GetFlagStatus(TIM4, TIM_FLAG_Update));
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);                            //Çå³ı±êÖ¾
  }
  TIM_Cmd(TIM4, DISABLE);                                            //¹Ø±Õ¶¨Ê±Æ÷
}

/************************************************
º¯ÊıÃû³Æ £º TIMDelay_Nms
¹¦    ÄÜ £º ¶¨Ê±Æ÷ÑÓÊ±Nms
²Î    Êı £º Times --- NÖµ
·µ »Ø Öµ £º ÎŞ
×÷    Õß £º acong
*************************************************/
void TIMDelay_Nms(uint16_t Times)
{
  while(Times--)
  {
    TIMDelay_N10us(100);
  }
}



void TIM3_PWM_Config(u16 arr,u16 psc)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//Ê¹ÄÜ¶¨Ê±Æ÷3Ê±ÖÓ
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //Ê¹ÄÜGPIOÍâÉè 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; //TIM_CH2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIO
  
  //³õÊ¼»¯TIM3
  TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üĞÂÊÂ¼ş×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊıÄ£Ê½
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //¸ù¾İTIM_TimeBaseInitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊıµ¥Î»
  
  //³õÊ¼»¯TIM3 Channel1 PWMÄ£Ê½	 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍß
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM3 OC2
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //Ê¹ÄÜTIM3ÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
  
  //³õÊ¼»¯TIM3 Channel2 PWMÄ£Ê½	 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍß
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM3 OC2
  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //Ê¹ÄÜTIM3ÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
  
  TIM_Cmd(TIM3, ENABLE);  //Ê¹ÄÜTIM3
	

}

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    isJam = 1;
  }
}

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
