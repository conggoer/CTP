/**
  ********************************  STM32F103RCT6  *********************************
  * @文件名     ： timer.c
  * @作者       ： acong
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年09月24日
  * @摘要       ： 超声波测距
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017年09月24日 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/*包含头文件---------------------------------------------------------------*/
#include "CSB.h"
/*变量定义------------------------------------------------------------------*/
extern float getai_height1, getai_height2, getai_height3;
unsigned char isJam;
/*函数声明---------------------------------------------------------------------*/
void sort( float *a, u8 l);
float CSB_height(void);
void get_getai_height1()
{
  float length11 = 0.0;
  float length12 = 0.0;
  float length13 = 0.0;
  unsigned char in_num = 0;
  float height1[10] ;
  u8 i;
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);			
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height1[i]=count/58.0;
  }
  sort(height1, 10);  //排序去掉两头的两个剩下的取平均值
  length11 = (height1[2]+height1[3]+height1[4]+height1[5]+height1[6]+height1[7])/6;
  if(length11>50)
    length11 = 0;
  else
    in_num++;
  
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_4);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOC,GPIO_Pin_4);
    
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height1[1]=count/58.0;
  }
  sort(height1, 10);  //排序去掉两头的两个剩下的取平均值
  length12 = (height1[2]+height1[3]+height1[4]+height1[5]+height1[6]+height1[7])/6;
  if(length12>50)
    length12 = 0;
  else
    in_num++;
  
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height1[i]=count/58.0;
  }
  sort(height1, 10);  //排序去掉两头的两个剩下的取平均值
  length13 = (height1[2]+height1[3]+height1[4]+height1[5]+height1[6]+height1[7])/6;
  if(length13>50)
    length13 = 0;
  else
    in_num++;
  if(in_num>0)
    getai_height1=(length11+length12+length13)/in_num;
  else
    getai_height1 = 0;

}

void get_getai_height2()
{
  float length21 = 0.0;
  float length22 = 0.0;
  float length23 = 0.0;
  unsigned char in_num = 0;
  u8 i;
  float height2[10];
  for(i = 0; i < 10; i++)
  {
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0) && (isJam==0));//等待ECHO脚高电平		
    TIM2->CNT=0;//计数器清0		
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);    
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height2[i]=count/58.0;
    if(isJam==1)
    {
      height2[i] = 0;
      isJam = 0;
    }
  }
  sort(height2, 10);  //排序去掉两头的两个剩下的取平均值
  length21 = (height2[2]+height2[3]+height2[4]+height2[5]+height2[6]+height2[7])/6;
  if(length21>50)
    length21 = 0;
  else
    in_num++;
  
  for(i = 0; i < 10; i++)
  {
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOB,GPIO_Pin_12);  
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM2->CNT=0;//计数器清0		
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE); 
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height2[i]=count/58.0;
    if(isJam==1)
    {
      height2[i] = 0;
      isJam = 0;
    }
  }	
  sort(height2, 10);  //排序去掉两头的两个剩下的取平均值
  length22 = (height2[2]+height2[3]+height2[4]+height2[5]+height2[6]+height2[7])/6;
  if(length22>50)
    length22 = 0;
  else
    in_num++;
  
  for(i = 0; i < 10; i++)
  {
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]       
    GPIO_SetBits(GPIOB,GPIO_Pin_14);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOB,GPIO_Pin_14);  
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM2->CNT=0;//计数器清0		
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE); 
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height2[i]=count/58.0;
    if(isJam==1)
    {
      height2[i] = 0;
      isJam = 0;
    }
  }
  sort(height2, 10);  //排序去掉两头的两个剩下的取平均值
  length23 = (height2[2]+height2[3]+height2[4]+height2[5]+height2[6]+height2[7])/6;
  if(length23>50)
    length23 = 0;
  else
    in_num++;
  if(in_num>0)
    getai_height2=(length21+length22+length23)/in_num;
  else
    getai_height2 = 0;	
}
void get_getai_height3()
{
  float length31 = 0.0;
  float length32 = 0.0;
  float length33 = 0.0;
  unsigned char in_num = 0;
  u8 i;
  float height3[10];
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_6);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
    
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height3[i]=count/58.0;
  }
  sort(height3, 10);  //排序去掉两头的两个剩下的取平均值
  length31 = (height3[2]+height3[3]+height3[4]+height3[5]+height3[6]+height3[7])/6;
  if(length31>50)  
    length31 = 0;
  else
    in_num++;       
  
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_8);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOC,GPIO_Pin_8);	
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    height3[i] =count/58.0;
  }
  sort(height3, 10);  //排序去掉两头的两个剩下的取平均值
  length32 = (height3[2]+height3[3]+height3[4]+height3[5]+height3[6]+height3[7])/6;
  
  if(length32>50)
    length32 = 0;
  else
    in_num++;
  
  for(i = 0; i < 10; i++)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_12);
    TIMDelay_N10us(2);//拉高超过10us，发射超声波
    GPIO_ResetBits(GPIOC,GPIO_Pin_12);
    
    TIM2->CNT=0;//计数器清0
    while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==0)&&(isJam==0));//等待ECHO脚高电平		
    TIM_Cmd(TIM2, ENABLE);// TIM2 enable counter [允许tim2计数]		
    while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==1)&&(isJam==0));
    TIM_Cmd(TIM2, DISABLE);
    count=TIM2->CNT;//ECHO脚低电平后读取计数器的值，从而算出往返时间
    length33=count/58.0;
  }
  
  sort(height3, 10);  //排序去掉两头的两个剩下的取平均值
  length33 = (height3[2]+height3[3]+height3[4]+height3[5]+height3[6]+height3[7])/6;
  if(length33>50)
    length33 = 0;
  else
    in_num++;
  if(in_num>0)
    getai_height3=(length31+length32+length33)/in_num;
  else
    getai_height3 = 0;
			
}

/**定义一个有小到大的排序函数**/
void sort( float *a, u8 l)
{
  u8 i,j;
  float t;
  for(i = 0; i < l-1; i++)
  {
    for(j = i+1; j < l; j++)
    {
      if(a[i] > a[j])
      {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
      }
    }
  }
}

/**三组超声波距离处理函**/

float CSB_height()
{
  float h;
  u8 n = 0;
  get_getai_height1();
  get_getai_height2();
  get_getai_height3();
  if(getai_height1>4 & getai_height1<100) n++;
  else getai_height1 = 0;
  if(getai_height2>4 & getai_height2<100) n++;
  else getai_height2 = 0;
  if(getai_height3>4 & getai_height3<100) n++;
  else getai_height3 = 0;
  h = (getai_height1 + getai_height2 + getai_height3)/n;
  return h;
}


