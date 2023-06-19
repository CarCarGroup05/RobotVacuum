#ifndef ultraSoundDist.h
#define ultraSoundDist.h
#include "movingControl.h"
#include "delayHolder.h"

int duration = 0;   // 收發時間差
int temp = 0;       // 記錄到的數據
int dist[3] = {};   // 紀錄各感測器的距離

void getDist(int pin){
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  duration = pulseIn(pin+1, HIGH);
  dist[(pin - trigF) / 2] = (duration/2) / 29.1;
}

int getDistState(){   // 取得距離並返回state，做為是否需要轉彎的依據
  for(int i = 0; i < 3; i++){
    getDist(trigF + i * 2);
    holdDelay(50);
  }

  if(abs(dist[1]) <= 6 || abs(dist[2]) <= 6){
    if(abs(dist[1]) < abs(dist[2]))
      return 1;
    else
      return 2;
  }

  if(abs(dist[0]) <= 6){
    if(started){
      MotorInverter();
      holdDelay(200);
    }
    if(abs(dist[1]) < abs(dist[2]))
      return 1;
    else
      return 2;
  }
  return 0;
}

#endif