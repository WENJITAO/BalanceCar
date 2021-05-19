#ifndef  __ESP8266_H__
#define  __ESP8266_H__
#define   uchar unsigned char
#define   uint unsigned int	

#include "stm32f10x.h"

extern unsigned char TX_DATA[8];
extern u8 fore,back,left,right,Res;
extern int turn,speed;

void USART2_Configuration(void);
void USART2_IRQHandler(void);
void usart_res_pid(void);

void esp8266_init(void);
void DATA_printf(uchar *s,float temp_data);
void USART2_SendData(uchar SendData);
void Send_data(void);
void printf_data(float data);
void printf_str(char *string);
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime);
void TIM3_Int_Init(u16 arr,u16 psc);
void wifi_init(void);

#endif
