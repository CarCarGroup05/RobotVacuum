#ifndef ultraSoundDist.h
#define ultraSoundDist.h

unsigned long long lastWater, startWater;
unsigned long long waterDuration = 1000;

void getDistState(){   // 取得距離並返回state，做為是否需要轉彎的依據

  for(int i = 0; i < 3; i++){
    digitalWrite(trigF + i * 2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigF + i * 2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigF + i * 2, LOW);
    duration = pulseIn(echoF + i * 2, HIGH);
    dist[i] = (duration/2) / 29.1;
  }
  // if
}
#endif