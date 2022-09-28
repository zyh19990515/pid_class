#include<Arduino.h>
#include <pid.h>

pid::pid(double Kp,double Ki,double Kd){
    kp = Kp;
    ki = Ki;
    kd = Kd;

    Bias = 0;
    //Last_Bias = 0;
}

int pid::balance(float Angle){
    static float Last_Bias,Integration,Balance_Integration,Flag_Target;  //上一次的偏差值
    int balance;
   Bias=(Angle-Zero_X);  //===求出平衡的角度中值 和机械相关  
   Differential=Bias-Last_Bias;  //求得偏差的变化率  
  if(++Flag_Target>20) //错频处理积分控制
  {
   Flag_Target=0;
   if(Flag_Stop==0) Integration+=Bias;  // 检测到小球且舵机使能则积分
   else Integration=0;//否则清零
   if(Integration<-200) Integration=-200; //积分限幅
   if(Integration>200)  Integration=200;  
   Balance_Integration=Integration*ki;  //积分控制
  }   
   balance=kp*Bias/500+kd*Differential/50+Balance_Integration;   //===计算平衡控制的舵机PWM  PD控制   kp是P系数 kd是D系数 
   Last_Bias=Bias;  //保存上一次的偏差
   return balance;  //返回值
}