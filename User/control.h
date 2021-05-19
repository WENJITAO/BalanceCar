#ifndef  __CONTROL_H__
#define  __CONTROL_H__

#include "stm32f10x.h"


extern int moto1,moto2;
extern float speed_Kp,speed_Ki;

extern float erect_kp ,erect_kd,MOD; 
extern float turn_kp,turn_kd;
extern int PWM_out,l;
extern int erect_out,speed_out,turn_out;
extern int Encoder_Left,Encoder_Right;

extern float Pitch,Roll,Yaw;						//???¨¨
extern short gyrox,gyroy,gyroz;				//¨ª¨®?Y¨°?--???¨´?¨¨
extern short aacx,aacy,aacz;	

void TIM3_IRQHandler(void);   //TIM3ÖÐ¶Ï

#endif
