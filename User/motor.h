#ifndef  __MOTOR_H__
#define  __MOTOR_H__

#include "stm32f10x.h"

void moto_control_gpioinit(void);
void Limit(int *motoa,int *motob);
void Load(int moto1,int moto2);


#endif
