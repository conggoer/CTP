/**
  ********************************  STM32F103RCT6  *********************************
  * @文件名     ： PID.h
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： PID控制算法
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _PID_H
#define _PID_H

/*定义PID结构体变量----------------------------------------------------------*/
typedef struct PID
{
	float SetPoint; // 设定目标Desired Value
	//float SetPoint1;
//	float SetPoint2;
	float Proportion; // 比例系数 Proportional Const
	float Integral; // 积分系数Integral Const
	float Derivative; // 微分系数Derivative Const
	float LastError; // 上次偏差
	float PrevError; // 上上次偏差
	float SumError; // 历史累计误差
} PID;

/* 函数申明 ------------------------------------------------------------------*/
void PIDRun(void);
float PIDCalc( PID *pp, float NextPoint );

#endif
