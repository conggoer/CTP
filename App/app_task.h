/**
  ********************************  STM32F103RCT6  *********************************
  *********************************  FreeRTOS  *********************************
  * @�ļ���     �� app_task.h
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @ϵͳ�汾   �� V9.0.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� Ӧ�ó�������ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _APP_TASK_H
#define _APP_TASK_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "can.h"
#include "usart.h"
#include "adc.h"   
#include "PID.h"
#include "CSB.h"
/* �궨�� --------------------------------------------------------------------*/


/* �������� ------------------------------------------------------------------*/
void Task_Measure(void *pvParameters);
void Task_Control(void *pvParameters);
void Task_Communication(void *pvParameters);
void Task_Inquiry(void *pvParameters);



#endif /* _APP_TASK_H */


