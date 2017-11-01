/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： adc.h
  * @作者       ： strongerHuang
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月06日
  * @摘要       ： ADC头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _ADC_H
#define _ADC_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"


void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 void TIMDelay_Nms(uint16_t Times);

#endif /* _ADC_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/

