#ifndef movingControl.h
#define movingControl.h

extern int _Tp;

void MotorWriting(double vL, double vR) {
  // TB6612
  analogWrite(MotorL_PWML, vL);
  analogWrite(MotorR_PWMR, vR);
}

// Handle negative motor_PWMR value.
void MotorInverter() {
  digitalWrite(MotorR_I1, 0);
  digitalWrite(MotorR_I2, 1);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp); 
}// MotorInverter

void MotorMove(){ // go straight
  digitalWrite(MotorR_I1, 1);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp); 
}

void backTurn(){ // back turn
  delay(60);
  MotorInverter();
  delay(600);
  MotorMove();
}

void lrTurn(int n){ // left trun 
  delay(150);
  MotorWriting(_Tp * (1 - ((n+1)%2)*2) / 6.5, _Tp * (1-((n)%2)*2) / 6.5);
  delay(680);
  MotorMove();
}

void MotorTunuing(int dist[]){
  MotorWriting(_Tp, _Tp);
}

#endif
