#ifndef movingControl.h
#define movingControl.h
#include "delayHolder.h"

extern int _Tp;

void MotorWriting(double vL, double vR) {
  analogWrite(MotorL_PWML, vL + 30);
  analogWrite(MotorR_PWMR, vR);
}

void MotorAhead(int L, int R){
  digitalWrite(MotorR_I1, 1);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(L, R); 
}// MotorInverter

// Handle negative motor_PWMR value.
void MotorInverter() {
  digitalWrite(MotorR_I1, 0);
  digitalWrite(MotorR_I2, 1);
  digitalWrite(MotorL_I3, 0);
  digitalWrite(MotorL_I4, 1);
  MotorWriting(_Tp, _Tp); 
}

void MotorMove(){ // go straight
  digitalWrite(MotorR_I1, 1);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp); 
}

void MotorStop(){
  digitalWrite(MotorR_I1, 0);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 0);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp); 
}

void rightAvoidance(){
  MotorInverter();
  holdDelay(400);
  MotorAhead(0, _Tp);
  holdDelay(980);
  MotorMove();
}

void leftAvoidance(){
  MotorInverter();
  holdDelay(400);
  MotorAhead(_Tp, 0);
  holdDelay(980);
  MotorMove();
}

void motorNext(int state){
  BT.write(state);
  BT.write("\n");
  switch(state){
    case 0:
      break;
    case 1:
      leftAvoidance();
      break;
    case 2:
      rightAvoidance();
      break;
    default:
      break;
  }
}
#endif
