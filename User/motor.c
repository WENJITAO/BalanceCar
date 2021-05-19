#include "motor.h"

void moto_control_gpioinit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;                       //声明gpio
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//限幅
int pwm_max=7200,pwm_min=-7200;
void Limit(int *motoa,int *motob)
{
   if(*motoa>pwm_max) *motoa=pwm_max;
   if(*motoa<pwm_min) *motoa=pwm_min;
   
   if(*motob>pwm_max) *motob=pwm_max;
   if(*motob<pwm_min) *motob=pwm_min;

}

//电机控制
void Load(int moto1,int moto2)
{    
	   if(moto1>=0)  {PAout(11)=1;PAout(12)=0;}
	   else          {PAout(11)=0;PAout(12)=1;moto1=-moto1;}
	   if(moto2>=0)  {PBout(3)=0;PBout(4)=1;}
	   else          {PBout(3)=1;PBout(4)=0;moto2=-moto2;}
	   TIM1->CCR1=moto1;
     TIM1->CCR2=moto2; 
}

