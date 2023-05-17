/***************************************************************************/
// File			  [track.h]
// Author		  [Erik Kuo]
// Synopsis		[Code used for tracking]
// Functions  [MotorWriting, MotorInverter, tracking, backTurn, lrTurn]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 1 & 6*/
/*feel free to add your own function for convenience*/
#ifndef track.h
#define track.h
/*===========================import variable===========================*/
int tempIR = 0;// 五個IR的讀值總和
extern int _Tp;
/*===========================import variable===========================*/

void MotorWriting(double vL, double vR) {
  // TODO: use TB6612 to control motor voltage & direction
  analogWrite(MotorL_PWML, vL);
  analogWrite(MotorR_PWMR, vR);
}// MotorWriting

// Handle negative motor_PWMR value.
void MotorInverter() {
  //Hint: the value of motor_PWMR must between 0~255, cannot write negative value.
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
  holdDelay(60);
  MotorInverter();
  askDelay(600);
  MotorMove();
}

void lrTurn(int n){ // left trun 
  delay(150);
  MotorWriting(_Tp * (1 - ((n+1)%2)*2) / 6.5, _Tp * (1-((n)%2)*2) / 6.5);
  askDelay(680);
  MotorMove();
}

void motionSwitch(char nextMo){
  switch(nextMo){
      case 'S': // stop
        holdDelay(120);
        MotorWriting(0, 0);
        delay(100000);
        break;

      case 'M': // move on
        MotorWriting(_Tp, _Tp);
        askDelay(400);
        MotorMove();
        break;
      case 'R': // turn right
        lrTurn(0);
        break;
      case 'L': // turn left
        lrTurn(1);
        break;
      case 'B': // back turn
        backTurn();
        break;
      default:
        break;
    }
}
#endif
