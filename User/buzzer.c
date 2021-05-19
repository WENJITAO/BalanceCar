#include "buzzer.h"

void buzzergpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;                       //声明gpio
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void bibi(void)
{
	PBout(5)=1;
}
