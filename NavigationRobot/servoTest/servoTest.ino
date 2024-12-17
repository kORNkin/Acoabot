#include <Servo.h>


Servo servo;

Servo rf;
Servo rm;
Servo rb;
Servo lf;
Servo lm;
Servo lb;

String in;
bool ch = false;

void setup() {
  Serial.begin(9600);

  //servo.attach(10);

  rf.attach(13);
  rm.attach(12);
  rb.attach(11);
  lf.attach(10);
  lm.attach(9);
  lb.attach(8);

  //servo.write(90);
}

void loop() {
  //setServoInput();
  //servoTest();
  setServo();
}

void setServo(){
  rf.write(180);
  rm.write(135);
  rb.write(104);
  lf.write(90);
  lm.write(45);
  lb.write(0);
}

void servoTest(){
  rf.write(0);
  rm.write(0);
  rb.write(0);
  lf.write(0);
  lm.write(0);
  lb.write(0);

  delay(1000);

  rf.write(45);
  rm.write(45);
  rb.write(45);
  lf.write(45);
  lm.write(45);
  lb.write(45);

  delay(1000);

  rf.write(90);
  rm.write(90);
  rb.write(90);
  lf.write(90);
  lm.write(90);
  lb.write(90);

  delay(1000);

  rf.write(180);
  rm.write(180);
  rb.write(180);
  lf.write(180);
  lm.write(180);
  lb.write(180);

   delay(1000);
}

void setServoInput(){
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
