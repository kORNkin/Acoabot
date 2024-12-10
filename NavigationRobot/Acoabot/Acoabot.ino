//#include <util/atomic.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x08
#include <2Servo.h>
#include <string>

///Encoder motor
//R
#define ENCB_RF 22 // WHITE
#define ENCA_RF 24 // YELLOW

#define ENCA_RM 26 
#define ENCB_RM 28 

#define ENCA_RB 30 
#define ENCB_RB 32
//L
#define ENCA_LF 34
#define ENCB_LF 36 

#define ENCA_LM 38 
#define ENCB_LM 40

#define ENCA_LB 42
#define ENCB_LB 44

///Motor dirver
//R
#define PWM_RF 13
#define IN1_RF 23
#define IN2_RF 25

#define PWM_RM 12
#define IN1_RM 27
#define IN2_RM 29

#define PWM_RB 11
#define IN1_RB 31
#define IN2_RB 33
//L
#define PWM_LF 10
#define IN1_LF 35
#define IN2_LF 37

#define PWM_LM 9
#define IN1_LM 39
#define IN2_LM 41

#define PWM_LB 8
#define IN1_LB 43
#define IN2_LB 45

#define relayPin 49
bool light = 0;

//Define servos
Servo servo_RF;
Servo servo_RM;
Servo servo_RB;

Servo servo_LF;
Servo servo_LM;
Servo servo_LB;

/*
FL 102
ML 95
BL 75
*/

//define servo offset values
int RF_ofst = 12;
int RM_ofst = 5;
int RB_ofst = -15;

int LF_ofst = 12;
int LM_ofst = 5;
int LB_ofst = -15;

//define servo starting position
int start_angle = 90;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 1);  
  light = 0;

  pinMode(ENCA_RF,INPUT);
  pinMode(ENCB_RF,INPUT);

  pinMode(ENCA_RM,INPUT);
  pinMode(ENCB_RM,INPUT);

  pinMode(ENCA_RB,INPUT);
  pinMode(ENCB_RB,INPUT);

  pinMode(ENCA_LF,INPUT);
  pinMode(ENCB_LF,INPUT);

  pinMode(ENCA_LM,INPUT);
  pinMode(ENCB_LM,INPUT);

  pinMode(ENCA_LB,INPUT);
  pinMode(ENCB_LB,INPUT);

  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_LF,OUTPUT);
  pinMode(IN1_LF,OUTPUT);
  pinMode(IN2_LF,OUTPUT);

  pinMode(PWM_LM,OUTPUT);
  pinMode(IN1_LM,OUTPUT);
  pinMode(IN2_LM,OUTPUT);

  pinMode(PWM_LB,OUTPUT);
  pinMode(IN1_LB,OUTPUT);
  pinMode(IN2_LB,OUTPUT);

  pinMode(PWM_RF,OUTPUT);
  pinMode(IN1_RF,OUTPUT);
  pinMode(IN2_RF,OUTPUT);

  pinMode(PWM_RM,OUTPUT);
  pinMode(IN1_RM,OUTPUT);
  pinMode(IN2_RM,OUTPUT);

  pinMode(PWM_RB,OUTPUT);
  pinMode(IN1_RB,OUTPUT);
  pinMode(IN2_RB,OUTPUT);

  //setup for servos
  servo_RF.attach(7);
  servo_RM.attach(6);
  servo_RB.attach(5);

  servo_LF.attach(4);
  servo_LM.attach(3);
  servo_LB.attach(2);

  servo_RF.write(start_angle + RF_ofst);
  servo_RM.write(start_angle + RM_ofst);
  servo_RB.write(start_angle + RB_ofst);

  servo_LF.write(start_angle + LF_ofst);
  servo_LM.write(start_angle + LM_ofst);
  servo_LB.write(start_angle + LB_ofst);

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
    }
    else if(inputData.startsWith("z")){
      rotateright();
      Serial.println("RR");
    }
    else if(inputData.startsWith("x")){
      rotateleft();
      Serial.println("RL");
    }
    else if(inputData.startsWith("f")){
      if(light){
        digitalWrite(relayPin, 0);
        Serial.println("No Flash");
      }else {
        digitalWrite(relayPin, 1);
        Serial.println("Flash!");
      } 
      light = !light;
    }else if(inputData.startsWith("FF")){
      setMotor(-1, 255, PWM_LF, IN1_LF, IN2_LF);
    }else if(inputData.startsWith("MM")){
      setMotor(-1, 255, PWM_LM, IN1_LM, IN2_LM);
    }else if(inputData.startsWith("BB")){
      setMotor(-1, 255, PWM_LB, IN1_LB, IN2_LB);
    }else if(inputData.startsWith("F")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_LF.write(tmp.toInt());
    }else if(inputData.startsWith("M")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_LM.write(tmp.toInt());
    }else if(inputData.startsWith("B")){
      String tmp = "";
      for(int i = 1; i < inputData.length(); i++){
        tmp += inputData[i];
      }
      servo_LB.write(tmp.toInt());
    }
  }

  inputData = "";
  stringComplete = false;

  delay(10);

  if(Serial.available() > 0) readControl();
}

void forward() {
  // Run the motor clockwise
  setMotor(-1, 255, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 255, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 255, PWM_RB, IN1_RB, IN2_RB);

  setMotor(-1, 255, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 255, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 255, PWM_LB, IN1_LB, IN2_LB);
}

void reverse() {
  // Run the motor counter-clockwise
  setMotor(1, 255, PWM_RF, IN1_RF, IN2_RF);
  setMotor(1, 255, PWM_RM, IN1_RM, IN2_RM);
  setMotor(1, 255, PWM_RB, IN1_RB, IN2_RB);

  setMotor(1, 255, PWM_LF, IN1_LF, IN2_LF);
  setMotor(1, 255, PWM_LM, IN1_LM, IN2_LM);
  setMotor(1, 255, PWM_LB, IN1_LB, IN2_LB);
}

void straight(){
  servo_RF.write(start_angle + RF_ofst);
  servo_RM.write(start_angle + RM_ofst);
  servo_RB.write(start_angle + RB_ofst);

  servo_LF.write(start_angle + LF_ofst);
  servo_LM.write(start_angle + LM_ofst);
  servo_LB.write(start_angle + LB_ofst);
}

void right(){
  servo_LF.write(130 + RF_ofst);
  servo_LM.write(110 + RM_ofst);
  servo_LB.write(90 + RB_ofst);

  servo_LF.write(130 + LF_ofst);
  servo_LM.write(110 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void left(){
  servo_LF.write(50 + RF_ofst);
  servo_LM.write(70 + RM_ofst);
  servo_LB.write(90 + RB_ofst);

  servo_LF.write(50 + LF_ofst);
  servo_LM.write(70 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void smallRight(){
  servo_LF.write(105 + RF_ofst);
  servo_LM.write(95 + RM_ofst);
  servo_LB.write(90 + RB_ofst);

  servo_LF.write(105 + LF_ofst);
  servo_LM.write(95 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void smallLeft(){
  servo_LF.write(75 + RF_ofst);
  servo_LM.write(85 + RM_ofst);
  servo_LB.write(90 + RB_ofst);

  servo_LF.write(75 + LF_ofst);
  servo_LM.write(85 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void rotateright() {
  servo_LF.write(20 + RF_ofst);
  servo_LM.write(90 + RM_ofst);
  servo_LB.write(160 + RB_ofst);

  servo_LF.write(20 + LF_ofst);
  servo_LM.write(90 + LM_ofst);
  servo_LB.write(160 + LB_ofst);

  delay(15);

  setMotor(-1, 255, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 127, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 255, PWM_RB, IN1_RB, IN2_RB);

  setMotor(-1, 255, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 127, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 255, PWM_LB, IN1_LB, IN2_LB);
}


void rotateleft() {
  servo_LF.write(20 + RF_ofst);
  servo_LM.write(90 + RM_ofst);
  servo_LB.write(160 + RB_ofst);

  servo_LF.write(20 + LF_ofst);
  servo_LM.write(90 + LM_ofst);
  servo_LB.write(160 + LB_ofst);

  delay(15);

  setMotor(1, 255, PWM_RF, IN1_RF, IN2_RF);
  setMotor(1, 127, PWM_RM, IN1_RM, IN2_RM);
  setMotor(1, 255, PWM_RB, IN1_RB, IN2_RB);

  setMotor(1, 255, PWM_LF, IN1_LF, IN2_LF);
  setMotor(1, 127, PWM_LM, IN1_LM, IN2_LM);
  setMotor(1, 255, PWM_LB, IN1_LB, IN2_LB);
}

void stopMotor() {
  // Stop the motor
  setMotor(0, 0, PWM_RF, IN1_RF, IN2_RF);
  setMotor(0, 0, PWM_RM, IN1_RM, IN2_RM);
  setMotor(0, 0, PWM_RB, IN1_RB, IN2_RB);

  setMotor(0, 0, PWM_LF, IN1_LF, IN2_LF);
  setMotor(0, 0, PWM_LM, IN1_LM, IN2_LM);
  setMotor(0, 0, PWM_LB, IN1_LB, IN2_LB);
}

void diagonalRight() {
  servo_LF.write(120 + RF_ofst);
  servo_LM.write(120 + RM_ofst);
  servo_LB.write(120 + RB_ofst);

  servo_LF.write(120 + LF_ofst);
  servo_LM.write(120 + LM_ofst);
  servo_LB.write(120 + LB_ofst);

  delay(15);
}

void diagonalLeft() {
  servo_LF.write(60 + RF_ofst);
  servo_LM.write(60 + RM_ofst);
  servo_LB.write(60 + RB_ofst);

  servo_LF.write(60 + LF_ofst);
  servo_LM.write(60 + LM_ofst);
  servo_LB.write(60 + LB_ofst);

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

