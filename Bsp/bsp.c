/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： bsp.c
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： BSP板级支持包源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-09-24 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
函数名称 ： RCC_Configuration
功    能 ： 时钟配置
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void RCC_Configuration(void)
{
  /* 使能APB2时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                           RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF  | 
                             RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE); 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能由APB1
}

/************************************************
函数名称 ： GPIO_Basic_Configuration
功    能 ： 基本输入输出引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //超声波
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          //选择输入模式为下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //输出频率最大50Hz
  GPIO_Init(GPIOB,&GPIO_InitStructure);

}

/************************************************
函数名称 ： NVIC_Configuration
功    能 ： NVIC配置
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 优先级分组 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* 外设中断 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //主优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 //从优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

/************************************************
函数名称 ： BSP_Initializes
功    能 ： 底层初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void BSP_Initializes(void)
{
  RCC_Configuration();                                               //时钟配置
  GPIO_Basic_Configuration();                                        //IO配置

  NVIC_Configuration();                                              //中断配置
}
