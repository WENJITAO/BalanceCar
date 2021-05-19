// STM32F103C8T6 的ADC1 通道有10路，分别是：
// 10 脚PA0 ADC12_IN0
// 11 脚PA1 ADC12_IN1
// 12 脚PA2 ADC12_IN2
// 13 脚PA3 ADC12_IN3
// 14 脚PA4 ADC12_IN4
// 15 脚PA5 ADC12_IN5
// 16 脚PA6 ADC12_IN6
// 17 脚PA7 ADC12_IN7
// 18 脚PA8 ADC12_IN8
// 19 脚PA9 ADC12_IN9
// ADC12 意为可配成ADC1或ADC2,不是有ADC12
//以下是main.c 程序；
#include "adc.h"


float adc_data=0,dyzh=0; //采集值变量，电压显示值变量

void ADC_GPIO_Init(void)                            //ADC-GPIO输入引脚配置,在此可以设置10路外部输入通道
{ 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能AHB预分频器到端口A的开关
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//引脚复用 进行重映射时要开启AFIO 时钟

GPIO_InitTypeDef GPIO_InitStructure;                                              //声明结构变量	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;             //定义PA0，PA1脚为AD输入 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //IO口为模拟输入模式
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;            //AD口设置为浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //按以上参数设置A口 A0/A1 
}

//******************************************************************************************** 
void ADC1_Init(void) //配置ADC参数及采集
{

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能AHB预分频器到外设ADC1的开关
RCC_ADCCLKConfig(RCC_PCLK2_Div8) ;                                   //时钟分频72M/8=9M 最大时钟不超过14M
ADC_DeInit(ADC1);                                                                        //ADC复位
ADC_DMACmd(ADC1, DISABLE);                                                //禁止DMA

ADC_InitTypeDef ADC_InitStuctrue;                                              //声明ADC结构变量	
ADC_InitStuctrue.ADC_Mode = ADC_Mode_Independent;          //ADC 独立工作模式 
ADC_InitStuctrue.ADC_ScanConvMode = DISABLE;                   //是否扫描_单通道
ADC_InitStuctrue.ADC_ContinuousConvMode = DISABLE;         //是否连续_单次
ADC_InitStuctrue.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //不用外部触发，软件触发转换
ADC_InitStuctrue.ADC_DataAlign=ADC_DataAlign_Right;           //数据右对齐
ADC_InitStuctrue.ADC_NbrOfChannel = 1;                                   //结构体_ADC-通道个数_单通道
//ADC_InitStuctrue.ADC_ScanDirection=ADC_ScanDirection_Backward; //数据覆盖/浏览方向
ADC_Init(ADC1,&ADC_InitStuctrue);                                            //按以上参数设置ADC1

ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_1Cycles5); 
//配置内容和ADC外设的具体对接函数 (ADC端口1 ,ADC通道0 ,转换序号-第1个转换 ,转换的周期) 
ADC_Cmd(ADC1,ENABLE);                                                        //使能ADC1
ADC_ResetCalibration(ADC1);                                                    //ADC寄存器复位校准函数 
while(ADC_GetResetCalibrationStatus(ADC1));                         //等待上一步完成
ADC_StartCalibration(ADC1);                                                     //开始指定ADC的校准状态
while(ADC_GetCalibrationStatus(ADC1));                                  //等待上一步完成 
}
//****************************************************************************************** 

   u8 i=0;
   float sum=0;
//****************************************************************************************** 
//****************************************************************************************** 
float adc_read(void)
{

;
   //delay_ms();
   ADC_SoftwareStartConvCmd(ADC1, ENABLE); //开关_ADC软件触发-开关 状态寄存器为0 
   while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);    //等待ADC完成
   adc_data=ADC_GetConversionValue(ADC1);    //所得数据即为ADC数据

   sum+=((adc_data*3.27)/4095)*4.09;                               //转换采集值为电压值//参考电压3.27V，12位分辨率4095
   i++;
   if(i==50)
   {
     dyzh=sum/50;
     i=0;
     sum=0;
   } 
   return  dyzh;                                                                         //此处可以设断点，查看上面dyzh值
}
