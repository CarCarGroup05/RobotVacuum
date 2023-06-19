#ifndef PiSerial.h
#define PiSerial.h

unsigned long nowAlert, durationAlert = 10000, startAlert = 0, tempAlert;
extern char BTchar;
extern bool inAlert;
extern String alertTemp;

void alertBuzz(){
  nowAlert = millis();
  if(!inAlert){
    if(Serial.available()){
      alertTemp = Serial.readStringUntil('\n');
      Serial.println(alertTemp);
      if(alertTemp == "H"){
        tone(alertBuzzer, 1000);
        startAlert = millis();
      }
    }
  }else{
    tempAlert = nowAlert - startAlert;
    if(tempAlert >= durationAlert){
      noTone(alertBuzzer);
      inAlert = false;
    }else{
      if(((tempAlert/250)+1)%2)
        tone(alertBuzzer, 1000);
      else
        noTone(alertBuzzer);
    }
  }
}

#endif