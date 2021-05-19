#include "esp8266.h"


/*wifi模块使用端口为PA9 PA10*/

u8 fore,back,left,right;         
u8 Res,key,j=0;
unsigned char TX_DATA[8];  	 //显示据缓存区
int turn=0,speed=0;

void printf_DMA(u8 *P,u32 L);
#define BUF_LEN 1000

u8 USART2_RX_BUF[BUF_LEN]={0}; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[BUF_LEN]={0}; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

u32 buff_length;

u8 USART2_flag=0;

void USART2_DMA_Rx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&(USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)USART2_RX_BUF;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize=BUF_LEN;
	DMA_InitStructure.DMA_PeripheralInc=DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	
	DMA_ClearFlag(DMA1_FLAG_TC6);
	
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TE,ENABLE);
	
	DMA_Cmd(DMA1_Channel6,ENABLE);
}

/*
********************************************************************************
** 函数名称 ： USART2_Configuration(void)
** 函数功能 ： 串口2初始化--wifi模块使用
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void USART2_Configuration(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef  USART_ClockInitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE  ); //|RCC_APB2Periph_AFIO |RCC_APB2Periph_TIM1|RCC_APB2Periph_USART1
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE  );
   /* Configure USART1 Tx (PA.09) as alternate function push-pull   |RCC_APB2Periph_TIM1   */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //	选中管脚9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);				 // 选择A端口
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			  //选中管脚10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);				  //选择A端口
  
  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
  /* Configure the USART1 synchronous paramters */
  USART_ClockInit(USART2, &USART_ClockInitStructure);					// 时钟参数初始化设置
																	 
  USART_InitStructure.USART_BaudRate = 115200;						  // 波特率为：115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
  USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
  /* Configure USART1 basic and asynchronous paramters */
  USART_Init(USART2, &USART_InitStructure);
    
  /* Enable USART1 */
  USART_ClearFlag(USART2, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	
	USART2_DMA_Rx_Config();
	
  USART_Cmd(USART2, ENABLE);							//USART1总开关：开启 
}

void Receive_datapack(void)
{
	
	DMA_Cmd(DMA1_Channel6,DISABLE);
	
	DMA_ClearFlag(DMA1_FLAG_TC6);
	
	buff_length=BUF_LEN-DMA_GetCurrDataCounter(DMA1_Channel6);
	
	DMA1_Channel6->CNDTR=BUF_LEN;
	
	DMA_Cmd(DMA1_Channel6,ENABLE);
	
}

//串口2中断处理函数
void USART2_IRQHandler(void)                	
{
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
    {
			Receive_datapack();
			USART2_flag=1;
			
			//printf_DMA(USART2_RX_BUF,buff_length);
			
			Res=USART_ReceiveData(USART2);

        if(Res=='0')     fore=0,back=0,left=0,right=0;//停
        else if(Res=='1')fore=1,back=0,left=0,right=0;//前进                    
        else if(Res=='2')fore=0,back=1,left=0,right=0;//后退
        else if(Res=='3')fore=0,back=0,left=1,right=0;//右转
        else if(Res=='4')fore=0,back=0,left=0,right=1;//左转
        else             fore=0,back=0,left=0,right=0;//停
			
        if(Res=='+')
        {key=1;
        printf_str("open pid regulate\r\n");}
        else if(Res=='-')
        {key=0;
         printf_str("close pid regulate\r\n");
         j=0;}
        if(key==1)usart_res_pid();
		}
}   		 
    
void usart_res_pid(void)
{
          if(j==1)
        {if(Res=='u')erect_kp+=30;
          if(Res=='d')erect_kp-=30;}
         if(j==2)
        {if(Res=='u')erect_kd+=0.1;
          if(Res=='d')erect_kd-=0.1;}
         if(j==3)
        {if(Res=='u')MOD+=0.1;
          if(Res=='d')MOD-=0.1;}
         if(j==4)
        {if(Res=='u')speed_Kp+=0.01;       
          if(Res=='d')speed_Kp-=0.01;}
         if(j==5)
        {if(Res=='u')turn_kp+=0.1;
          if(Res=='d')turn_kp-=0.1;}
         if(j==6)
        {if(Res=='u')turn_kd+=0.1;
          if(Res=='d')turn_kd-=0.1;}     				
           switch(Res)
           {
           case 'P':j=1;break;
           case 'D':j=2;break;
           case 'M':j=3;break;
           case 'p':j=4;break; 
           case 'T':j=5;break;
           case 't':j=6;break;					 
           }  
}
 
void response_judge(u8 *buf)
{
	//No AP
}

//esp8266连接热点向同热点下服务器发送数据
void esp8266_init(void)
{
 unsigned char espinit_1data[20]="AT+CWMODE=3";      //设置为 softAP+station 共存模式
 uchar j;
 for(j=0;j<11;j++)
 USART2_SendData(espinit_1data[j]);
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(10);
 
  unsigned char espinit_2data[10]="AT+RST";          //重启生效
  j=0;
 for(j=0;j<6;j++)
 USART2_SendData(espinit_2data[j]); 
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(300);
 
  unsigned char espinit_3data[20]="AT+CIPMUX=0";
 j=0;
 for(j=0;j<11;j++)
 USART2_SendData(espinit_3data[j]);
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(10);
 
 
 unsigned char espinit_4data[40]="AT+CWJAP=\"HONOR 9X\",\"123456789\"";  //连接手机热点
 //unsigned char espinit_4data[40]="AT+CWJAP=\"wen\",\"123456789\"";  //连接路由器
 //unsigned char espinit_4data[40]="AT+CWJAP=\"ChinaNet-VYRz\",\"aenldvlj\"";  //连接路由器
 j=0;
 for(j=0;j<31;j++)
 //for(j=0;j<26;j++)
  //for(j=0;j<35;j++)
 USART2_SendData(espinit_4data[j]);
 USART2_SendData(0X0D);	 //换
 USART2_SendData(0X0A);	 //回车
 delay_ms(1800);
 delay_ms(1800);
 delay_ms(1800);
 delay_ms(1800);
 delay_ms(1000);
	
 
   //unsigned char espinit_5data[40]="AT+CIPSTART=\"TCP\",\"192.168.1.16\",8080"; //电脑
   unsigned char espinit_5data[40]="AT+CIPSTART=\"TCP\",\"192.168.43.1\",1234";//手机//传入协议、服务器 IP、端口号
  //unsigned char espinit_5data[50]="AT+CIPSTART=\"TCP\",\"29g0359b81.51vip.biz\",54960";//手机//传入协议、服务器 IP、端口号
 j=0;
 //for(j=0;j<37;j++)
 for(j=0;j<37;j++)
 //  for(j=0;j<46;j++)
 USART2_SendData(espinit_5data[j]);
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(200);
 
   unsigned char espinit_6data[20]="AT+CIPMODE=1";         //开启透传模式
 j=0;
 for(j=0;j<12;j++)
 USART2_SendData(espinit_6data[j]);
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(10);
 
    unsigned char espinit_7data[20]="AT+CIPSEND";         // 开始透传
 j=0;
 for(j=0;j<10;j++)
 USART2_SendData(espinit_7data[j]);
 USART2_SendData(0X0D);	 //换行
 USART2_SendData(0X0A);	 //回车
 delay_ms(10);
 
 printf_str("wifi connection is successful!\r\n");
}




/* 变量定义 ----------------------------------------------*/

  /*******************************/
void DATA_printf(uchar *s,float temp_data)
{
  int k,u;
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
        k=(int)(temp_data*100);
        
    *++s =k/100000+0x30;
    if(*s=='0')
    {s=s-1;u=1;}
    k=k%100000;   
    *++s =k/10000+0x30;
    if((*s=='0')&&(u==1))
    {s=s-1;u=1;}
    k=k%10000;     //取余运算
    *++s =k/1000+0x30;
    if((*s=='0')&&(u==1))    
    {s=s-1;u=1;}
    k=k%1000;      //取余运算
    *++s =k/100+0x30;
    *++s ='.';
    k=k%100;     //取余运算
    *++s =k/10+0x30;
    k=k%10;      //取余运算
    *++s =k+0x30;    
}

 //************************************************
void  USART2_SendData(uchar SendData)
{
USART_SendData(USART2, SendData);
while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

 //********串口发送数据***************************************
 void Send_data(void)
 {
   uchar i;
  for(i=0;i<8;i++)USART2_SendData(TX_DATA[i]);
  USART2_SendData(' ');
  USART2_SendData(' ');
 }

//发送数据
void printf_data(float data)
{       

	DATA_printf(TX_DATA,data);//转换X轴数据到数组
        Send_data();
	USART2_SendData(0X0D);	 //换行
        delay_ms(2);
	USART2_SendData(0X0A);	 //回车

}

void printf_str(char *string)
{
   uchar j,len;
   len=strlen(string);
   for(j=0;j<len;j++)
     {
       USART2_SendData(*string++);    
     }
}

void printf_DMA(u8 *P,u32 L)
{
	 uchar j;
	
   for(j=0;j<L;j++)
     {
       USART2_SendData(*P++);    
     }

}
