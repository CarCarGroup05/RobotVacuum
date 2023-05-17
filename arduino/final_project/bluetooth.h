/***************************************************************************/
// File			  [bluetooth.h]
// Author		  [Erik Kuo]
// Synopsis		[Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

#ifndef bluetooth_h
#define bluetooth_h

char inpTemp[10]; // 儲存起始指令的 buffer

bool ask_BT(char &nextMove){ // get command from python
  if(BT.available()){
    BT.readBytes(inpTemp, 10);
    nextMove = inpTemp[0];
    // Serial.println(nextMove);
    return true;
  }
  return false;
}

bool askStart(){
  if(BT.available()){
      BT.readBytes(inpTemp, 10);
      if(inpTemp[0] == 'Q')// if receive 'Q' from python
        return true;       // , then the car start running
      else
        return false;
  }
  return false;
}

bool askNext(char& nextMo){
  unsigned long s1, e1;
  s1 = millis();
  if(BT.available()){
    e1 = millis(); 
    Serial.println((e1-s1));
    nextMo = BT.read();
    Serial.print(nextMo);
    return true;
  }
  return false;
}

// send UID back through Serial1(bluetooth serial)
void send_byte(byte *id, byte& idSize, bool newlyFound){
  if(newlyFound){
    for (byte i = 0; i < 4; i++) {  // Send UID consequently.
        BT.write(id[i]); // send id to python
        Serial.print(id[i],HEX); // print id in arduino serail monitor 
    }
    Serial.println();
    newlyFound = false;
  }
}
#endif
