/**
  ********************************  STM32F103RCT6  *********************************
  * @文件名     ： timer.c
  * @作者       ： acong
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： 定时器源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017年09月24日 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "timer.h"

extern unsigned char isJam;
/************************************************
函数名称 ： TIMER_Initializes
功    能 ： TIMER初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void TIMER_Initializes(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* RCC时钟配置 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* TIM4时基单元配置 */
  TIM_TimeBaseStructure.TIM_Prescaler = TIM4_PRESCALER_VALUE;        //预分频值
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //向上计数模式
  TIM_TimeBaseStructure.TIM_Period = TIM4_PERIOD_TIMING;             //定时周期
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //时钟分频因子
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* 使能预分频值 */
  TIM_ARRPreloadConfig(TIM4, ENABLE);                                //使能重载值
}


void Tim2_Init(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

   TIM_DeInit(TIM2);//复位TIM2定时器
   TIM_TimeBaseStructure.TIM_Period = 49999;             //定时时间50ms,计算公式：定时时间（us） = （（1+TIM_Prescaler）/72M）*(1+TIM_Period)
   TIM_TimeBaseStructure.TIM_Prescaler = 71; 
   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
     
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);
     
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/************************************************
函数名称 ： TIMDelay_N10us
功    能 ： 定时器延时N个10us
参    数 ： Times --- N值
返 回 值 ： 无
作    者 ： acong
*************************************************/
void TIMDelay_N10us(uint16_t Times)
{
  TIM_Cmd(TIM4, ENABLE);                                             //启动定时器
  while(Times--)
  {
    while(RESET == TIM_GetFlagStatus(TIM4, TIM_FLAG_Update));
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);                            //清除标志
  }
  TIM_Cmd(TIM4, DISABLE);                                            //关闭定时器
}

/************************************************
函数名称 ： TIMDelay_Nms
功    能 ： 定时器延时Nms
参    数 ： Times --- N值
返 回 值 ： 无
作    者 ： acong
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
  
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; //TIM_CH2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
  
  //初始化TIM3
  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
  //初始化TIM3 Channel1 PWM模式	 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低�
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
  
  //初始化TIM3 Channel2 PWM模式	 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低�
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
  
  TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

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
