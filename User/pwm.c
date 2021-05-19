#include "pwm.h"
/**********************************/
//��ʼ��pwm
//��ͨ��PA 8 9 
//ʹ�ö�ʱ��TIM1
/**********************************/
//..............................................................................
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 	
  GPIO_InitTypeDef GPIO_InitStruct;                       //����gpio
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;         //������ʱ��
	TIM_OCInitTypeDef TIM_OCInitStruct;                     //����pwmͨ��
        
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//��ʼ����ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;//��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);       
		
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
        
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//��ͨ��Ԥװ�ؼĴ���ʹ��
  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1,ENABLE);

	TIM_Cmd(TIM1,ENABLE);//ʹ��tim1
        
 
} 
//..............................................................................

