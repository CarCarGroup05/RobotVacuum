/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
// #include "node.h" // redundant

/*===========================define pin & create module object================================*/
// BlueTooth
// Mega          HC05
// Pin (Function) Pin
// 10    TX   ->   RX
// 11    RX   <-   TX
SoftwareSerial BT(10, 11);

// 馬達控制器(TB6612), 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
#define MotorR_I1 2    //定義 A1 接腳（右）
#define MotorR_I2 3    //定義 A2 接腳（右）
#define MotorR_PWMR 9  //定義 ENA (PWM調速) 接腳
#define MotorL_I3 5    //定義 B1 接腳（左）
#define MotorL_I4 6    //定義 B2 接腳（左）
#define MotorL_PWML 8  //定義 ENB (PWM調速) 接腳

// 紅外線模組(IR模組), 請按照自己車上的接線寫入腳位
#define IRpin_LL 32
#define IRpin_L 34
#define IRpin_M 36
#define IRpin_R 38
#define IRpin_RR 40

// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 7                  // 讀卡機的重置腳位
#define SS_PIN 53                  // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup(){
  //bluetooth initialization
  BT.begin(9600);
  //Serial window
  Serial.begin(9600);
  //RFID initial
  SPI.begin();
  mfrc522.PCD_Init();

  //TB6612(motor) pin
  pinMode(MotorR_I1, OUTPUT);
  pinMode(MotorR_I2, OUTPUT);
  pinMode(MotorL_I3, OUTPUT);
  pinMode(MotorL_I4, OUTPUT);
  pinMode(MotorL_PWML, OUTPUT);
  pinMode(MotorR_PWMR, OUTPUT);
  digitalWrite(MotorR_I1, 1);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);

  //tracking(IR) pin
  pinMode(IRpin_LL, INPUT);
  pinMode(IRpin_L, INPUT);
  pinMode(IRpin_M, INPUT);
  pinMode(IRpin_R, INPUT);
  pinMode(IRpin_RR, INPUT);
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "track.h"
#include "bluetooth.h"
/*=====Import header files=====*/

/*=======5====================initialize variables===========================*/
int _Tp = 250;  // set your own value for motor power
int mapState = 0;
bool start = false;
bool received = false;
char stepTemp[10], nextMove;
int irRead = 0;
byte idSize = 8; // store size of id
bool atNode = true; // to check whether the car is at node or not
bool newlyFound = false; // flag set as true when find a new TREASURE
bool motorInitializer = false;
int test = 15;
unsigned long startTime, endTime;
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/

void Search();  // search graph
void tracking(char);
void holdDelay(int);
void askDelay(int);

/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
void loop(){
  // MotorWriting(_Tp, _Tp);
  while(!start){
    send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
    while(!received){
      received = ask_BT(nextMove);
    }
    if(askStart()){
      MotorMove();
      delay(400);
      start = true;
    }
  }
  // use send_byte() function to send uid to python
  // use rfid() function to read uid
  send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
  // search() is a function to start search 
  Search();
}

/*===========================define function===========================*/
void Search(){
  if(nextMove == "B")
    send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
  // flag newlyFound determines whether to send UID
  tracking(nextMove);
}

void tracking(char nextMo){
  tempIR = 0;
  atNode = true;
  for(int i = 0; i < 5; i++){
    if(!digitalRead(32 + 2 * i))
      atNode = false;
    tempIR += (i - 2)*digitalRead(32 + 2 * i);
  }
  MotorWriting(_Tp* (1 + tempIR * 0.1 - abs(tempIR) * 0.1), _Tp * (1 - tempIR * 0.1 - abs(tempIR) * 0.1));
  if(atNode)
    motionSwitch(nextMo);
}

void holdDelay(int time){ // delay and search for RFIDs simultaneously
  for(int i = 0; i < time/20; i++){
    delay(20);
    send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
  }
}

void askDelay(unsigned long time){
  bool nextStep = false;
  startTime = millis();
  endTime = millis();
  BT.write(test);
  while((time - (endTime - startTime)) <= 10000 && !nextStep){
    nextStep = askNext(nextMove);
    endTime = millis();
    if(nextStep){
      break;
    }
  }
  endTime = millis();
  Serial.println((endTime - startTime));
  if((time - (endTime - startTime)) < time)
    delay((time - (endTime - startTime)));
}

/*===========================define function===========================*/