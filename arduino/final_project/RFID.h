/***************************************************************************/
// File       [RFID.h]
// Author     [Erik Kuo]
// Synopsis   [Code for getting UID from RFID card]
// Functions  [rfid]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

/*===========================don't change anything in this file===========================*/

#include <SPI.h>
#include <MFRC522.h>     // 引用程式庫
/* pin---- SDA:9 SCK:13 MOSI:11 MISO:12 GND:GND RST:define on your own  */
byte none = 0;
byte* rfid(byte& idSize, bool& newlyFound) {
  Serial.println("reading");
    // 確認是否有新卡片
  if(!mfrc522.PICC_IsNewCardPresent()) {
    newlyFound = false;
    idSize = none;
    goto FuncEnd;
  } // confirm whether RFID detected a new card or not

  if(!mfrc522.PICC_ReadCardSerial()) {
    newlyFound = false;
    idSize = none;
    goto FuncEnd;
  }

  byte *id = mfrc522.uid.uidByte;   // git UID of a card
  idSize = mfrc522.uid.size;   // get the UID size
  mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
  newlyFound = true;
  FuncEnd:; // goto here
  return id; // return type : byte*
}

// if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
// Serial.print("PICC type: ");      // 顯示卡片類型
// 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
// MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
// Serial.println(mfrc522.PICC_GetTypeName(piccType));
// #ifdef DEBUG
// Serial.print("UID Size: ");       // 顯示卡片的UID長度值
// #endif
// Serial.println();
// Serial.println(idSize);
// for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
//   Serial.print("id[");
//   Serial.print(i);
//   Serial.print("]: ");
//   Serial.println(id[i], HEX);       // 以16進位顯示UID值  
// }
// }
// Serial.println(newlyFound);
