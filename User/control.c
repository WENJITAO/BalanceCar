#include "control.h"

float Pitch,Roll,Yaw;
short gyrox,gyroy,gyroz;			
short aacx,aacy,aacz;					

float MOD=-3,t;               //机械中值
float erect_kp=400,erect_kd=1.86;//400  1.86
float speed_Kp=-0.32,speed_Ki=-0.0016;  //-0.32
float turn_kp=35,turn_kd=0.8;  //30 0.8

int erect_out,speed_out,turn_out;
int moto1,moto2;//,Num=0;
char buf[20];
int PWM_out;
int flag_up=0;
int Encoder_Left=0,Encoder_Right=0;

static int Encoder_S;
int PID_erect(float med,float angle,float gyro_x);
int PID_turn(float gyro_z,int RC);
int PID_speed(int speed,int encoder_left,int encoder_right);

//定时器3中断服务程序
void EXTI9_5_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{   
                  if(PAin(5)==0)
									{										                       
                    EXTI_ClearITPendingBit(EXTI_Line5);
										
										mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//角度
										MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪
										MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度
                     			
                     Encoder_Left=-Read_Speed(2);
		                 Encoder_Right=Read_Speed(4);
										
										
										if(flag_up==1)t++;
										if(t==500){t=0;flag_up=0;Encoder_S=0;}

										                      //前后
                     if((fore==0)&&(back==0))speed=0;//&&(Long>0)
                     if(fore==1)speed--;
                     if((back==1))speed++;//||(Long<0 )
										 if(fore==1||back==1)Encoder_S=0;
                     speed=speed>80?80:(speed<(-80)?(-80):speed);
                   
                     //左右
                     if((left==0)&&(right==0))turn=0;
                     if(left==1)turn++;
                     if(right==1)turn--;
                     turn=turn>50?50:(turn<(-50)?(-50):turn);  
                     
                     if((left==0)&&(right==0))turn_kd=1.8;        
                     else if((left==1)||(right==1))turn_kd=0;     									
										 
										 if(Roll<50&&Roll>(-50)&&flag_up==0)
										 {
                     speed_out=PID_speed(speed,Encoder_Left,Encoder_Right);
					           erect_out=PID_erect(speed_out+MOD,Roll,gyrox);
                     turn_out= PID_turn(gyroz,turn);  							 
      
                     moto1=erect_out+turn_out;
                     moto2=erect_out-turn_out;
      
                     Limit(&moto1,&moto2);
                     Load(moto1,moto2);     
										 }
										else
										{
										Load(0,0);
										flag_up=1;
										}											
                  }
  }
}


//直立环    med -期望角度， angle -真实角度  gyro_x -角速度
int PID_erect(float med,float angle,float gyro_x)
{
    int pwm_out;
    pwm_out=(int)(erect_kp*(angle-med)+erect_kd*(gyro_x-0));
    
    return pwm_out; 
}

//速度环
int PID_speed(int speed,int encoder_left,int encoder_right)
{
	static int pwm_out,Encoder_Err,EnC_Err_Lowout,EnC_Err_Lowout_last;
	float a=0.7;


	//1.计算速度偏差
	Encoder_Err=(encoder_left+encoder_right)-speed;
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(int)((1-a)*Encoder_Err+a*EnC_Err_Lowout_last);
	EnC_Err_Lowout_last=EnC_Err_Lowout;
	//3.对速度偏差进行积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	//5.速度环控制输出
	pwm_out=(int)(speed_Kp*EnC_Err_Lowout+speed_Ki*Encoder_S);
	return pwm_out;

}


//转向环   gyro_z——z轴角速度
int PID_turn(float gyro_z,int RC)
{
   int pwm_out;
   pwm_out=(int)(turn_kd*gyro_z+turn_kp*RC);
   return pwm_out;

}
