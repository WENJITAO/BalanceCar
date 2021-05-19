#ifndef  __ADC_H__
#define  __ADC_H__

#include "stm32f10x.h"

void ADC_GPIO_Init(void);
void ADC1_Init(void);
float adc_read(void);



#endif
