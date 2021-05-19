// STM32F103C8T6 ��ADC1 ͨ����10·���ֱ��ǣ�
// 10 ��PA0 ADC12_IN0
// 11 ��PA1 ADC12_IN1
// 12 ��PA2 ADC12_IN2
// 13 ��PA3 ADC12_IN3
// 14 ��PA4 ADC12_IN4
// 15 ��PA5 ADC12_IN5
// 16 ��PA6 ADC12_IN6
// 17 ��PA7 ADC12_IN7
// 18 ��PA8 ADC12_IN8
// 19 ��PA9 ADC12_IN9
// ADC12 ��Ϊ�����ADC1��ADC2,������ADC12
//������main.c ����
#include "adc.h"


float adc_data=0,dyzh=0; //�ɼ�ֵ��������ѹ��ʾֵ����

void ADC_GPIO_Init(void)                            //ADC-GPIO������������,�ڴ˿�������10·�ⲿ����ͨ��
{ 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��AHBԤ��Ƶ�����˿�A�Ŀ���
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//���Ÿ��� ������ӳ��ʱҪ����AFIO ʱ��

GPIO_InitTypeDef GPIO_InitStructure;                                              //�����ṹ����	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;             //����PA0��PA1��ΪAD���� 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //IO��Ϊģ������ģʽ
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;            //AD������Ϊ��������
GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //�����ϲ�������A�� A0/A1 
}

//******************************************************************************************** 
void ADC1_Init(void) //����ADC�������ɼ�
{

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��AHBԤ��Ƶ��������ADC1�Ŀ���
RCC_ADCCLKConfig(RCC_PCLK2_Div8) ;                                   //ʱ�ӷ�Ƶ72M/8=9M ���ʱ�Ӳ�����14M
ADC_DeInit(ADC1);                                                                        //ADC��λ
ADC_DMACmd(ADC1, DISABLE);                                                //��ֹDMA

ADC_InitTypeDef ADC_InitStuctrue;                                              //����ADC�ṹ����	
ADC_InitStuctrue.ADC_Mode = ADC_Mode_Independent;          //ADC ��������ģʽ 
ADC_InitStuctrue.ADC_ScanConvMode = DISABLE;                   //�Ƿ�ɨ��_��ͨ��
ADC_InitStuctrue.ADC_ContinuousConvMode = DISABLE;         //�Ƿ�����_����
ADC_InitStuctrue.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //�����ⲿ�������������ת��
ADC_InitStuctrue.ADC_DataAlign=ADC_DataAlign_Right;           //�����Ҷ���
ADC_InitStuctrue.ADC_NbrOfChannel = 1;                                   //�ṹ��_ADC-ͨ������_��ͨ��
//ADC_InitStuctrue.ADC_ScanDirection=ADC_ScanDirection_Backward; //���ݸ���/�������
ADC_Init(ADC1,&ADC_InitStuctrue);                                            //�����ϲ�������ADC1

ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_1Cycles5); 
//�������ݺ�ADC����ľ���ԽӺ��� (ADC�˿�1 ,ADCͨ��0 ,ת�����-��1��ת�� ,ת��������) 
ADC_Cmd(ADC1,ENABLE);                                                        //ʹ��ADC1
ADC_ResetCalibration(ADC1);                                                    //ADC�Ĵ�����λУ׼���� 
while(ADC_GetResetCalibrationStatus(ADC1));                         //�ȴ���һ�����
ADC_StartCalibration(ADC1);                                                     //��ʼָ��ADC��У׼״̬
while(ADC_GetCalibrationStatus(ADC1));                                  //�ȴ���һ����� 
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
   ADC_SoftwareStartConvCmd(ADC1, ENABLE); //����_ADC�������-���� ״̬�Ĵ���Ϊ0 
   while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);    //�ȴ�ADC���
   adc_data=ADC_GetConversionValue(ADC1);    //�������ݼ�ΪADC����

   sum+=((adc_data*3.27)/4095)*4.09;                               //ת���ɼ�ֵΪ��ѹֵ//�ο���ѹ3.27V��12λ�ֱ���4095
   i++;
   if(i==50)
   {
     dyzh=sum/50;
     i=0;
     sum=0;
   } 
   return  dyzh;                                                                         //�˴�������ϵ㣬�鿴����dyzhֵ
}
