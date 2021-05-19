#ifndef  __HC_SR04_H__
#define  __HC_SR04_H__

#include "stm32f10x.h"

extern float Long;

void hc_sr04_init(void);
void hc_sr04_start(void);
void hc_sr04_open(void);

#endif
