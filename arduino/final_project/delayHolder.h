#ifndef delayHolder.h
#define delayHolder.h
#include "Bluetooth.h"

unsigned long tempDelay, startDelay;
bool endDelay;
extern bool started;

void holdDelay(int n){
  startDelay = millis();
  endDelay = false;
  while(!endDelay){
    tempDelay = millis() - startDelay;
    if(tempDelay > n)
      endDelay = true;
    else{
      // alertBuzz();
      if(started)
        bluetoothStop();
      else
        bluetoothRec();
    }
  }
}

#endif