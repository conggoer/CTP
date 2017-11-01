/**
  ********************************  STM32F103RCT6  *********************************
  * @文件名     ： PID.h
  * @作者       ： acong
  * @标准库     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： PID控制算法
  ******************************************************************************/
#include "PID.h"
#include "timer.h"
#include "CSB.h"
#include "stdlib.h"
#include "usart.h"

PID stPID; // 定义一个stPID变量
float set_height = 30.0;
unsigned short  P_set = 500,I_set = 2000,D_set = 3043;					  //设置整定参数值
float fout;
char dist_abs = 0; //距离差绝对值 
int duty = 0;     //占空比

/************************************************
函数名称 ： PIDCalc
功    能 ： PID输出值计算
参    数 ： *pp--自己定义的PID结构体名称  NextPoint--实际测量值
返 回 值 ： PID计算结果
作    者 ： acong
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
函数名称 ： PIDRun
功    能 ： 根据返回结果运行PID
参    数 ： 无
返 回 值 ： 无
作    者 ： acong
*************************************************/
void PIDRun()
{
  stPID.Proportion = (float)P_set*0.01; //设置PID比例值
  stPID.Integral = (float)I_set*0.01; //设置PID积分值
  stPID.Derivative = (float)D_set*0.01; //设置PID微分值
  stPID.SetPoint = (float)set_height; 
  fout = PIDCalc(&stPID,getai_height ); //PID 计算
  if( fout > 0 )
  {
    TIM_SetCompare2(TIM3,duty);
    TIM_SetCompare1(TIM3,0);
    //继续升高
  }
  else if( fout <= 0 )
  {
    TIM_SetCompare1(TIM3,duty);			   //割台降低
    TIM_SetCompare2(TIM3,0);
  }
 }