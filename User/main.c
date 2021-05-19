#include "stm32f10x.h"

int main(void)
{
	 delay_init();
	
	TIM1_PWM_Init(7199,0);
	moto_control_gpioinit();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	OLED_Init();
  OLED_Clear(); 

	drawbmp(1);	
	
  USART2_Configuration();	 
  delay_ms(5);
  
  esp8266_init();
  
  Encoder_TIM2_Init();
  
  Encoder_TIM4_Init();  
  
  ADC_GPIO_Init();              
  ADC1_Init();   
	
  hc_sr04_init();           //超声波

  NVIC_Configuration();  

OLED_Clear();
			
  while(1)
 {

    oled_open_re();         //主界面
	  oled_pid();							//调参界面
	  oled_adc();							//电量查询
    hc_sr04_open();
		bibi();
  }
}
