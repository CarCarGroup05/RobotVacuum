#ifndef Bluetooth.h
#define Bluetooth.h

extern int durationSecond;
extern bool mode[2];
extern bool started;
char BTtemp[5];
extern char BTchar;
int num;

void bluetoothRec(){
  if(BT.available()){
    BT.write("Hi");
    BT.readBytes(BTtemp, 5);
    // Serial.println(BTtemp[0]);
    switch(BTtemp[0]){
      case 'A':
        started = true;
        mode[0] = mode[1] = 0;
        break;
      case 'B':
        started = true;
        mode[0] = 0;
        mode[1] = 1;
        break;
      case 'C':
        started = true;
        mode[0] = 1;
        mode[1] = 0;
        break;
      case 'D':
        started = true;
        mode[0] = mode[1] = 1;
        break;
    case 'T':{
        num = 0;
        // Serial.print(BTtemp[0]);
        for(int i = 1; i < 3; i++){
          num *= 10;
          num += (BTtemp[i] - '0');
        }
        durationSecond = num;
        durationSecond *= 60;
        Serial.println(durationSecond);
        started = false;
        break;
      }
      default:
        started = false;
        break;
    }
  }
}

void bluetoothStop(){
  if(BT.available()){
    BTchar = BT.read();
    Serial.println(BTchar);
    if(BTchar == 'X')
      started = false;
  }
}

#endif