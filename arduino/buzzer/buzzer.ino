const int buzzer = 10; //buzzer to arduino pin 9

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {}

}

void loop() {
  if(Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.println(message);
    if(message == "H") {
      tone(buzzer, 1000);
      delay(500);
    }  
  }  
}
