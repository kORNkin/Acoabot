//#include <util/atomic.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x08
#include <ESP32Servo.h>
#include <string>

#define ENCA_R 19 // YELLOW
#define ENCB_R 18 // WHITE

#define PWM_FR 4
#define IN1_FR 2
#define IN2_FR 15

#define PWM_MR 25
#define IN1_MR 26
#define IN2_MR 27

#define PWM_BR 14
#define IN1_BR 12
#define IN2_BR 13

//Define servos
Servo servo_FR;
Servo servo_MR;
Servo servo_BR;

/*
FR 120
MR 128
BR 130
*/

//define servo offset values
int FR_ofst = 30;
int MR_ofst = 38;
int BR_ofst = 40;

//define servo starting position
int start_angle = 90;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(ENCA_R,INPUT);
  pinMode(ENCB_R,INPUT);

  // pinMode(ENCA_MR,INPUT);
  // pinMode(ENCB_MR,INPUT);
  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_MR,OUTPUT);
  pinMode(IN1_MR,OUTPUT);
  pinMode(IN2_MR,OUTPUT);

  pinMode(PWM_BR,OUTPUT);
  pinMode(IN1_BR,OUTPUT);
  pinMode(IN2_BR,OUTPUT);

  pinMode(PWM_FR,OUTPUT);
  pinMode(IN1_FR,OUTPUT);
  pinMode(IN2_FR,OUTPUT);

  //setup for servos
  servo_FR.attach(23);
  servo_MR.attach(32);
  servo_BR.attach(35);

  servo_FR.write(start_angle + FR_ofst);
  servo_MR.write(start_angle + MR_ofst);
  servo_BR.write(start_angle + BR_ofst);
}

char incomingByte;

void receiveData(int byteCount) {
  while (Wire.available()) {
    incomingByte = Wire.read();

    switch (incomingByte) {
      case 'w':
        Serial.println("w");
        forward();
        break;
      case 's':
        Serial.println("s");
        reverse();
        break;

      default:
        stopMotor();
        break;
    }
  }
}

void sendData(){
  char buffer[50];
  Serial.println("Gu dai rub" + inputData);
}

void readControl(){
    while(Serial.available()) {
      char inputChar = (char)Serial.read();
      inputData += inputChar;
      if(inputChar == '\n'){
        stringComplete = true;
      }

    }
}

void loop() {
  if(stringComplete){
    if(inputData.startsWith("status")){
      sendData();
    }
    else if(inputData.startsWith("w")){
      forward();
      Serial.println("Forward");
    }else if(inputData.startsWith("s")){
      reverse();
      Serial.println("Reverse");
    }else if(inputData.startsWith("u")){
      smallLeft();
      Serial.println("Small Left");
    }else if(inputData.startsWith("o")){
      smallRight();
      Serial.println("Small Right");
    }else if(inputData.startsWith("j")){
      left();
      Serial.println("Left");
    }else if(inputData.startsWith("l")){
      right();
      Serial.println("Right");
    }else if(inputData.startsWith("k")){
      straight();
      Serial.println("Straight");
    }else if(inputData.startsWith("q")){
      stopMotor();
      Serial.println("Stop");
    }else if(inputData.startsWith("FF")){
      setMotor(1, 255, PWM_FR, IN1_FR, IN2_FR);
    }else if(inputData.startsWith("MM")){
      setMotor(1, 255, PWM_MR, IN1_MR, IN2_MR);
    }else if(inputData.startsWith("BB")){
      setMotor(1, 255, PWM_BR, IN1_BR, IN2_BR);
    }else if(inputData.startsWith("F")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_FR.write(tmp.toInt());
    }else if(inputData.startsWith("M")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_MR.write(tmp.toInt());
    }else if(inputData.startsWith("B")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_BR.write(tmp.toInt());
    }
  }

  inputData = "";
  stringComplete = false;

  delay(10);

  if(Serial.available() > 0) readControl();
}

void forward() {
  // Run the motor clockwise
  setMotor(1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(1, 255, PWM_MR, IN1_MR, IN2_MR);
  setMotor(1, 255, PWM_BR, IN1_BR, IN2_BR);
}

void reverse() {
  // Run the motor counter-clockwise
  setMotor(-1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(-1, 255, PWM_MR, IN1_MR, IN2_MR);
  setMotor(-1, 255, PWM_BR, IN1_BR, IN2_BR);
}

void straight(){
  servo_FR.write(start_angle + FR_ofst);
  servo_MR.write(start_angle + MR_ofst);
  servo_BR.write(start_angle + BR_ofst);
}

void right(){
  servo_FR.write(130 + FR_ofst);
  servo_MR.write(110 + MR_ofst);
  servo_BR.write(90 + BR_ofst);

  delay(15);
}

void left(){
  servo_FR.write(50 + FR_ofst);
  servo_MR.write(70 + MR_ofst);
  servo_BR.write(90 + BR_ofst);

  delay(15);
}

void smallRight(){
  servo_FR.write(105 + FR_ofst);
  servo_MR.write(95 + MR_ofst);
  servo_BR.write(90 + BR_ofst);

  delay(15);
}

void smallLeft(){
  servo_FR.write(75 + FR_ofst);
  servo_MR.write(85 + MR_ofst);
  servo_BR.write(90 + BR_ofst);

  delay(15);
}

void rotateright() {
  servo_FR.write(20 + FR_ofst);
  servo_MR.write(90 + MR_ofst);
  servo_BR.write(160 + BR_ofst);

  delay(15);

  setMotor(-1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(-1, 127, PWM_MR, IN1_MR, IN2_MR);
  setMotor(-1, 255, PWM_BR, IN1_BR, IN2_BR);
}


void rotateleft() {
  servo_FR.write(20 + FR_ofst);
  servo_MR.write(90 + MR_ofst);
  servo_BR.write(160 + BR_ofst);

  delay(15);

  setMotor(1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(1, 127, PWM_MR, IN1_MR, IN2_MR);
  setMotor(1, 255, PWM_BR, IN1_BR, IN2_BR);
}

void stopMotor() {
  // Stop the motor
  setMotor(0, 0, PWM_FR, IN1_FR, IN2_FR);
  setMotor(0, 0, PWM_MR, IN1_MR, IN2_MR);
  setMotor(0, 0, PWM_BR, IN1_BR, IN2_BR);
}

void diagonalRight() {
  servo_FR.write(120 + FR_ofst);
  servo_MR.write(120 + MR_ofst);
  servo_BR.write(120 + BR_ofst);

  delay(15);
}

void diagonalLeft() {
  servo_FR.write(60 + FR_ofst);
  servo_MR.write(60 + MR_ofst);
  servo_BR.write(60 + BR_ofst);

  delay(15);
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    //Serial.print("1 ");
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    //Serial.print("-1 ");
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

