#ifndef mopNVacuum.h
#define mopNVacuum.h
#include "delayHolder.h"

unsigned long long lastWater, nowTime;
unsigned long long waterDuration = 2000, waterInterval = 30000;

void brushing(int n){
  analogWrite(brushPin, n * 255);
  digitalWrite(brushRes, n);
}

void mopping(){         // 加水控制
  nowTime = millis();
  if(nowTime - lastWater >= waterInterval){
    analogWrite(mopPin, 255);
    lastWater = nowTime;
  }
  if(nowTime - lastWater >= waterDuration)
    analogWrite(mopPin, 0);
}

void mopStart(){        // 起始加水
  analogWrite(mopPin, 255);
  holdDelay(2000);
  analogWrite(mopPin, 0);
  lastWater = millis();
}

void mopStop(){         // 時間到要停
  analogWrite(mopPin, 0);
}

void vacuum(int n){     // 吸塵控制(只有開/關)
  digitalWrite(fanPin, n);
}

#endif