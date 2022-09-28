#include <Arduino.h>
#include<pid.h>
#include<Servo.h>

#define ZERO_X 133
#define ZERO_Y 100

#define YL        5    
#define YH        6  
#define XL        7  
#define XH        8
Servo myservo1,myservo2;
int Position_X,Position_Y; //X Y方向的测量值
float Zero_X=133,Zero_Y=100,Target_X,Target_Y;  //X Y方向的目标值和控制量
void(*resetFunc)(void)=0;
float Balance_Kp=53,Balance_Ki=0.6,Balance_Kd=58;//PID参数
pid pid_x(Balance_Kp,Balance_Ki,Balance_Kd);
pid pid_y(Balance_Kp,Balance_Ki,Balance_Kd);
void Control_servo(float velocity_x,float velocity_y);
void control();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(128000);
  
  while (!Serial)
  {
    delay(200);
  }
  
  pinMode(XL, OUTPUT);          //电机控制引脚
  pinMode(XH, OUTPUT);          //电机控制引脚，
  pinMode(YL, OUTPUT);          //电机速度控制引脚
  pinMode(YH, OUTPUT);          //电机速度控制引脚
  myservo1.attach(10);           //初始化各个舵机
  myservo2.attach(9);            //初始化各个舵机
  myservo1.write(94);
  myservo2.write(88);
  Serial.println("setup finished");
  for(int i=0;i<5;i++){
    Serial.println(i);
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  control();
  Serial.print("set_x:");
  Serial.print(Zero_X);
  Serial.print("    set_y:");
  Serial.print(Zero_Y);
  Serial.print("    PositionX:");
  Serial.print(Position_X);
  Serial.print("    PositionY");
  Serial.println(Position_Y);
}

void Control_servo(float velocity_x,float velocity_y)
{
    myservo1.write(90-velocity_x);        // X舵机转向的角度
    myservo2.write(90-velocity_y);        // Y舵机转向的角度
}
void control(){  

  static uint8_t Max_Target=30;
    
   
  digitalWrite(YL, LOW);  //给X方向+3.3V电压
  digitalWrite(YH, HIGH);   
  digitalWrite(XL, HIGH);  
  digitalWrite(XH, LOW); 
  Position_Y=analogRead(3)/5; //测量Y方向的坐标          
  digitalWrite(YL, HIGH);  
  digitalWrite(YH, LOW); 
  digitalWrite(XL, LOW);  
  digitalWrite(XH, HIGH); 
  delay(25);
  digitalWrite(YL, HIGH);  //给Y方向+3.3V电压//D3
  digitalWrite(YH, LOW); //  A0
  digitalWrite(XL, LOW);  //  D2
  digitalWrite(XH, HIGH); //  A1
  Position_X= analogRead(2)*4/15; //测量X方向的           
  digitalWrite(YL, LOW);  //
  digitalWrite(YH, HIGH); //  A0
  digitalWrite(XL, HIGH);  //  D2
  digitalWrite(XH, LOW); //  A1
    
  Target_X=-pid_x.balance(Position_X);   //X方向的PID控制器
  Target_Y=-pid_y.balance(Position_Y);   //Y方向的PID控制器
  if(Target_X<-Max_Target) Target_X=-Max_Target;  //X方向的舵机的控制最大角度
  if(Target_X>Max_Target)  Target_X=Max_Target;   //X方向的舵机的控制最大角度
  if(Target_Y<-Max_Target) Target_Y=-Max_Target;  //Y方向的舵机的控制最大角度
  if(Target_Y>Max_Target)  Target_Y=Max_Target;   //Y方向的舵机的控制最大角度
  Control_servo(Target_X,Target_Y); 
            
 }