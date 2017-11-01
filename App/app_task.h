/**
  ********************************  STM32F103RCT6  *********************************
  *********************************  FreeRTOS  *********************************
  * @文件名     ： app_task.h
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @系统版本   ： V9.0.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： 应用程序任务头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _APP_TASK_H
#define _APP_TASK_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "can.h"
#include "usart.h"
#include "adc.h"   
#include "PID.h"
#include "CSB.h"
/* 宏定义 --------------------------------------------------------------------*/


/* 函数申明 ------------------------------------------------------------------*/
void Task_Measure(void *pvParameters);
void Task_Control(void *pvParameters);
void Task_Communication(void *pvParameters);
void Task_Inquiry(void *pvParameters);



#endif /* _APP_TASK_H */


