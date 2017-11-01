/**
  ********************************  STM32F103RCT6  *********************************
  * @�ļ���     �� PID.h
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� PID�����㷨
  ******************************************************************************/
#include "PID.h"
#include "timer.h"
#include "CSB.h"
#include "stdlib.h"
#include "usart.h"

PID stPID; // ����һ��stPID����
float set_height = 30.0;
unsigned short  P_set = 500,I_set = 2000,D_set = 3043;					  //������������ֵ
float fout;
char dist_abs = 0; //��������ֵ 
int duty = 0;     //ռ�ձ�

/************************************************
�������� �� PIDCalc
��    �� �� PID���ֵ����
��    �� �� *pp--�Լ������PID�ṹ������  NextPoint--ʵ�ʲ���ֵ
�� �� ֵ �� PID������
��    �� �� acong
*************************************************/
float PIDCalc( PID *pp, float NextPoint )
{
  float dError,Error; 
  unsigned char index;
  Error = pp->SetPoint - NextPoint; 
  dist_abs = abs((int)Error);
  if(dist_abs<5)
  {
    pp->SumError += Error; 
    duty = 900*dist_abs ;
    printf("duty=%d\r\n ",duty);
    index = 1;
  }	
  else
  {
    duty = 4500;
    printf("duty=%d\r\n ",duty);
    index = 0;
  }					
  
  dError = pp->LastError - pp->PrevError; 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 
  
  return (pp->Proportion * Error
          + pp->Integral * pp->SumError*index * 0.01  
            + pp->Derivative * dError);
	
}

/************************************************
�������� �� PIDRun
��    �� �� ���ݷ��ؽ������PID
��    �� �� ��
�� �� ֵ �� ��
��    �� �� acong
*************************************************/
void PIDRun()
{
  stPID.Proportion = (float)P_set*0.01; //����PID����ֵ
  stPID.Integral = (float)I_set*0.01; //����PID����ֵ
  stPID.Derivative = (float)D_set*0.01; //����PID΢��ֵ
  stPID.SetPoint = (float)set_height; 
  fout = PIDCalc(&stPID,getai_height ); //PID ����
  if( fout > 0 )
  {
    TIM_SetCompare2(TIM3,duty);
    TIM_SetCompare1(TIM3,0);
    //��������
  }
  else if( fout <= 0 )
  {
    TIM_SetCompare1(TIM3,duty);			   //��̨����
    TIM_SetCompare2(TIM3,0);
  }
 }