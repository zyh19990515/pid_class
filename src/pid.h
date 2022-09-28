#ifndef pid_h
#define pid_h
 
 class pid
 {
 private:
    /* data */
    double kp;
    double ki;
    double kd;
    float Zero_X,Zero_Y;

    float Differential,Bias;
    //static float Last_Bias,Integration,Balance_Integration=0,Flag_Target=0;
    int Flag_Stop=1;

 public:
    pid(double,double,double);
    int balance(float Angle);
 };
 

 


#endif