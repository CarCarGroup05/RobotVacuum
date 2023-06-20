#define DEBUG  // debug flag
#include <SPI.h>
#include<SoftwareSerial.h>
SoftwareSerial BT(10, 11);// 40  TX -> RX

// 馬達控制器(TB6612)
#define MotorR_I1 32    //定義 A1 接腳（右）
#define MotorR_I2 33    //定義 A2 接腳（右）
#define MotorR_PWMR 12  //定義 ENA (PWM調速) 接腳
#define MotorL_I3 35    //定義 B1 接腳（左）
#define MotorL_I4 34    //定義 B2 接腳（左）
#define MotorL_PWML 13  //定義 ENB (PWM調速) 接腳

// 超音波感測器(HC_SR04)
// 前、左、右各有收發兩腳位
#define trigF 22 
#define echoF 23
#define trigL 24
#define echoL 25
#define trigR 26
#define echoR 27

// 按鈕
#define buttonM 49    // 拖地
#define buttonV 51    // 吸塵
#define buttonS 53    // 開始運行

#define mopPin 2
#define fanPin 3
#define brushPin 4
#define brushRes 48

// 蜂鳴器
#define alertBuzzer 45

void setup(){
  BT.begin(9600);
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

  // fan pin
  pinMode(mopPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(brushPin, OUTPUT);
  pinMode(brushRes, OUTPUT);

  //SuperSound pin
  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);

  //others
  pinMode(buttonV, INPUT);
  pinMode(buttonM, INPUT);
  pinMode(buttonS, INPUT);
  pinMode(alertBuzzer, OUTPUT);
  Serial.begin(9600);
  // while (!Serial) {}
}
/*############## Headers & variables ##############*/
#include "movingControl.h"
#include "mopNVacuum.h"
#include "ultraSoundDist.h"
#include "delayHolder.h"

unsigned long startTime;
int durationSecond = 1 * 60, startSecond; 
int _Tp = 170;         // 機器前進速度
bool mode[2] = {0, 0}; // 選擇模式
bool started = false;  // 開始運行的flag
bool inAlert = false;
char BTchar;
String alertTemp;
char temptemp[10];

/*############## Headers & variables ##############*/

void loop(){
  Serial.print(durationSecond);
  mode[0] = mode[1] = 0;
  while(!started){     // 選擇模式
    Serial.print(mode[0]);
    Serial.println(mode[1]);
    bluetoothRec();
    getDistState();
    for(int i =0; i < 2; i++){
      if(digitalRead(buttonM + 2 * i) == LOW){
        mode[i] = !mode[i];
        holdDelay(600);
      }
    }
    if(digitalRead(buttonS) == LOW)
      started = true;
  }
  Serial.println(durationSecond);
  // 開始運行
  Serial.println("Start");
  brushing(1);
  if(mode[1])
    mopStart();
  if(mode[0])
    vacuum(1);
  MotorMove();
  startTime = millis();

  while(started){
    startSecond = (millis() - startTime) / 1000;
    Serial.println(startSecond);
    bluetoothStop();
    
    if(startSecond >= durationSecond)
      started = false;
    else{
      motorNext(getDistState());
      if(mode[1])
        mopping();
    }
    if(!started){
      MotorStop();
      mopStop();
      vacuum(0);
      brushing(0);
      mode[0] = mode[1] = 0;
    }
  }
}