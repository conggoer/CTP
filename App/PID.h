/**
  ********************************  STM32F103RCT6  *********************************
  * @�ļ���     �� PID.h
  * @����       �� acong
  * @��׼��     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��09��24��
  * @ժҪ       �� PID�����㷨
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _PID_H
#define _PID_H

/*����PID�ṹ�����----------------------------------------------------------*/
typedef struct PID
{
	float SetPoint; // �趨Ŀ��Desired Value
	//float SetPoint1;
//	float SetPoint2;
	float Proportion; // ����ϵ�� Proportional Const
	float Integral; // ����ϵ��Integral Const
	float Derivative; // ΢��ϵ��Derivative Const
	float LastError; // �ϴ�ƫ��
	float PrevError; // ���ϴ�ƫ��
	float SumError; // ��ʷ�ۼ����
} PID;

/* �������� ------------------------------------------------------------------*/
void PIDRun(void);
float PIDCalc( PID *pp, float NextPoint );

#endif
