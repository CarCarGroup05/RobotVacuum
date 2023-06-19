#define DEBUG  // debug flag
// 馬達控制器(TB6612)
#define MotorR_I1 8    //定義 A1 接腳（右）
#define MotorR_I2 9    //定義 A2 接腳（右）
#define MotorR_PWMR 10  //定義 ENA (PWM調速) 接腳
#define MotorL_I3 11    //定義 B1 接腳（左）
#define MotorL_I4 12    //定義 B2 接腳（左）
#define MotorL_PWML 13  //定義 ENB (PWM調速) 接腳

// 超音波感測器(HC_SR04)
// 前、左、右各有收發兩腳位
#define trigF 22
#define echoF 23
#define trigL 24
#define echoL 25
#define trigR 26
#define echoR 27

// 掃地邊刷角位
#define brush 12       //控制邊刷(類比輸出)
#define button 34

void setup(){
  // TB6612(motor) pin
  pinMode(MotorR_I1, OUTPUT);
  pinMode(MotorR_I2, OUTPUT);
  pinMode(MotorL_I3, OUTPUT);
  pinMode(MotorL_I4, OUTPUT);
  pinMode(MotorL_PWML, OUTPUT);
  pinMode(MotorR_PWMR, OUTPUT);
  digitalWrite(MotorR_I1, 0);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 0);
  digitalWrite(MotorL_I4, 0);
  // brush pin
  pinMode(brush, OUTPUT);
  //SuperSound pin
  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  //others
  pinMode(button, INPUT);

  Serial.begin(9600);
}
/*############## Headers & variables ##############*/
#include "movingControl.h"
#include "ultraSoundDist.h"
int _Tp = 250;         //機器前進速度
// int dist[3] = {};      //記錄超音波測距數據
bool started = false;  //開始運行的flag
/*############## Headers & variables ##############*/

void loop(){
  while(!started)
    if(digitalRead(button) == HIGH){
      started = true;
      MotorMove();
      analogWrite(brush, 200);
  }

  // getDist(dist);
  // MotorTuning(dist);
}