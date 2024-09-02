//#include <util/atomic.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x08
#include <ESP32Servo.h>
#include <string>

#define ENCA_L 19 // YELLOW
#define ENCB_L 18 // WHITE

#define PWM_FL 4
#define IN1_FL 2
#define IN2_FL 15

#define PWM_ML 25
#define IN1_ML 26
#define IN2_ML 27

#define PWM_BL 14
#define IN1_BL 12
#define IN2_BL 13

//Define servos
Servo servo_FL;
Servo servo_ML;
Servo servo_BL;

/*
FL 102
ML 95
BL 75
*/

//define servo offset values
int FL_ofst = 12;
int ML_ofst = 5;
int BL_ofst = -15;

//define servo starting position
int start_angle = 90;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(ENCA_L,INPUT);
  pinMode(ENCB_L,INPUT);

  // pinMode(ENCA_ML,INPUT);
  // pinMode(ENCB_ML,INPUT);
  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_ML,OUTPUT);
  pinMode(IN1_ML,OUTPUT);
  pinMode(IN2_ML,OUTPUT);

  pinMode(PWM_BL,OUTPUT);
  pinMode(IN1_BL,OUTPUT);
  pinMode(IN2_BL,OUTPUT);

  pinMode(PWM_FL,OUTPUT);
  pinMode(IN1_FL,OUTPUT);
  pinMode(IN2_FL,OUTPUT);

  //setup for servos
  servo_FL.attach(23);
  servo_ML.attach(32);
  servo_BL.attach(5);

  servo_FL.write(start_angle + FL_ofst);
  servo_ML.write(start_angle + ML_ofst);
  servo_BL.write(start_angle + BL_ofst);
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
      setMotor(-1, 255, PWM_FL, IN1_FL, IN2_FL);
    }else if(inputData.startsWith("MM")){
      setMotor(-1, 255, PWM_ML, IN1_ML, IN2_ML);
    }else if(inputData.startsWith("BB")){
      setMotor(-1, 255, PWM_BL, IN1_BL, IN2_BL);
    }else if(inputData.startsWith("F")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_FL.write(tmp.toInt());
    }else if(inputData.startsWith("M")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_ML.write(tmp.toInt());
    }else if(inputData.startsWith("B")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_BL.write(tmp.toInt());
    }
  }

  inputData = "";
  stringComplete = false;

  delay(10);

  if(Serial.available() > 0) readControl();
}

void forward() {
  // Run the motor clockwise
  setMotor(-1, 255, PWM_FL, IN1_FL, IN2_FL);
  setMotor(-1, 255, PWM_ML, IN1_ML, IN2_ML);
  setMotor(-1, 255, PWM_BL, IN1_BL, IN2_BL);
}

void reverse() {
  // Run the motor counter-clockwise
  setMotor(1, 255, PWM_FL, IN1_FL, IN2_FL);
  setMotor(1, 255, PWM_ML, IN1_ML, IN2_ML);
  setMotor(1, 255, PWM_BL, IN1_BL, IN2_BL);
}

void straight(){
  servo_FL.write(start_angle + FL_ofst);
  servo_ML.write(start_angle + ML_ofst);
  servo_BL.write(start_angle + BL_ofst);
}

void right(){
  servo_FL.write(130 + FL_ofst);
  servo_ML.write(110 + ML_ofst);
  servo_BL.write(90 + BL_ofst);

  delay(15);
}

void left(){
  servo_FL.write(50 + FL_ofst);
  servo_ML.write(70 + ML_ofst);
  servo_BL.write(90 + BL_ofst);

  delay(15);
}

void smallRight(){
  servo_FL.write(105 + FL_ofst);
  servo_ML.write(95 + ML_ofst);
  servo_BL.write(90 + BL_ofst);

  delay(15);
}

void smallLeft(){
  servo_FL.write(75 + FL_ofst);
  servo_ML.write(85 + ML_ofst);
  servo_BL.write(90 + BL_ofst);

  delay(15);
}

void rotateright() {
  servo_FL.write(20 + FL_ofst);
  servo_ML.write(90 + ML_ofst);
  servo_BL.write(160 + BL_ofst);

  delay(15);

  setMotor(-1, 255, PWM_FL, IN1_FL, IN2_FL);
  setMotor(-1, 127, PWM_ML, IN1_ML, IN2_ML);
  setMotor(-1, 255, PWM_BL, IN1_BL, IN2_BL);
}


void rotateleft() {
  servo_FL.write(20 + FL_ofst);
  servo_ML.write(90 + ML_ofst);
  servo_BL.write(160 + BL_ofst);

  delay(15);

  setMotor(1, 255, PWM_FL, IN1_FL, IN2_FL);
  setMotor(1, 127, PWM_ML, IN1_ML, IN2_ML);
  setMotor(1, 255, PWM_BL, IN1_BL, IN2_BL);
}

void stopMotor() {
  // Stop the motor
  setMotor(0, 0, PWM_FL, IN1_FL, IN2_FL);
  setMotor(0, 0, PWM_ML, IN1_ML, IN2_ML);
  setMotor(0, 0, PWM_BL, IN1_BL, IN2_BL);
}

void diagonalRight() {
  servo_FL.write(120 + FL_ofst);
  servo_ML.write(120 + ML_ofst);
  servo_BL.write(120 + BL_ofst);

  delay(15);
}

void diagonalLeft() {
  servo_FL.write(60 + FL_ofst);
  servo_ML.write(60 + ML_ofst);
  servo_BL.write(60 + BL_ofst);

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

