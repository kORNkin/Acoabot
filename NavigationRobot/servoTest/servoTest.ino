#include <ESP32Servo.h>

Servo servo;

String in;
bool ch = false;

void setup() {
  Serial.begin(9600);

  servo.attach(23);

  //servo.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(servo.read());

  while(Serial.available() > 0){
      char inputChar = (char)Serial.read();
      in += inputChar;
      if(inputChar == '\n'){
        ch = true;
      }
  }

  if(ch == true){
    servo.write(in.toInt());
  }

  in = "";
  ch = false;
  
}

/*

FR 96
MR 102
BR 76

FL 120
ML 127
BL 130

*/
