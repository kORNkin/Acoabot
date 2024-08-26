//#include <util/atomic.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x08
#include <ESP32Servo.h>
#include <string>

#define ENCA 19 // YELLOW
#define ENCB 18 // WHITE

//#define ENCA_MR 19 // YELLOW
//#define ENCB_MR 18 // WHITE
#define PWM_MR 14
#define IN1_MR 12
#define IN2_MR 13

//#define ENCA_BR 4 // YELLOW
//#define ENCB_BR 2 // WHITE
#define PWM_BR 25
#define IN1_BR 26
#define IN2_BR 27

//#define ENCA_FR 19 // YELLOW
//#define ENCB_FR 18 // WHITE
#define PWM_FR 32
#define IN1_FR 35
#define IN2_FR 33

//Define servos
Servo servo_fl;
Servo servo_ml;
Servo servo_bl;
Servo servo_fr;
Servo servo_mr;
Servo servo_br;

//define servo offset values
int fr_ofst = 5; 
int mr_ofst = 4;
int br_ofst = -1;
int fl_ofst = -9;
int ml_ofst = 2;
int bl_ofst = -5;

//define servo starting position
int start_angle = 90;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);

  // pinMode(ENCA_MR,INPUT);
  // pinMode(ENCB_MR,INPUT);
  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_MR,OUTPUT);
  pinMode(IN1_MR,OUTPUT);
  pinMode(IN2_MR,OUTPUT);

  //pinMode(ENCA_BR,INPUT);
  //pinMode(ENCB_BR,INPUT);
  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_BR,OUTPUT);
  pinMode(IN1_BR,OUTPUT);
  pinMode(IN2_BR,OUTPUT);

  //pinMode(ENCA_FR,INPUT);
  //pinMode(ENCB_FR,INPUT);
  //attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(PWM_FR,OUTPUT);
  pinMode(IN1_FR,OUTPUT);
  pinMode(IN2_FR,OUTPUT);

  //setup for servos
  // servo_fl.attach(31);
  // servo_ml.attach(33);
  // servo_bl.attach(32);
  // servo_fr.attach(28);
  // servo_mr.attach(29);
  servo_br.attach(23);

  // servo_fl.write(start_angle+fl_ofst);
  // servo_ml.write(start_angle+ml_ofst);
  // servo_bl.write(start_angle+bl_ofst);
  // servo_fr.write(start_angle+fr_ofst);
  // servo_mr.write(start_angle+mr_ofst);
  servo_br.write(start_angle+br_ofst);
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
      Serial.println("Backward");
    }else if(inputData.startsWith("h")){
      stopMotor();
      Serial.println("Stop");
    }
  }

  inputData = "";
  stringComplete = false;

  delay(10);

  if(Serial.available() > 0) readControl();
}

void forward() {
  // Run the motor clockwise
  // setMotor(1, 255, PWM_FL, IN1_FL, IN2_FL);
  // setMotor(1, 255, PWM_ML, IN1_ML, IN2_ML);
  // setMotor(1, 255, PWM_BL, IN1_BL, IN2_BL);
  setMotor(1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(1, 255, PWM_MR, IN1_MR, IN2_MR);
  setMotor(1, 255, PWM_BR, IN1_BR, IN2_BR);
}

void reverse() {
  // Run the motor counter-clockwise
  // setMotor(-1, 255, PWM_FL, IN1_FL, IN2_FL);
  // setMotor(-1, 255, PWM_ML, IN1_ML, IN2_ML);
  // setMotor(-1, 255, PWM_BL, IN1_BL, IN2_BL);
  setMotor(-1, 255, PWM_FR, IN1_FR, IN2_FR);
  setMotor(-1, 255, PWM_MR, IN1_MR, IN2_MR);
  setMotor(-1, 255, PWM_BR, IN1_BR, IN2_BR);
}

void stopMotor() {
  // Stop the motor
  // setMotor(0, 0, PWM_FL, IN1_FL, IN2_FL);
  // setMotor(0, 0, PWM_ML, IN1_ML, IN2_ML);
  // setMotor(0, 0, PWM_BL, IN1_BL, IN2_BL);
  setMotor(0, 0, PWM_FR, IN1_FR, IN2_FR);
  setMotor(0, 0, PWM_MR, IN1_MR, IN2_MR);
  setMotor(0, 0, PWM_BR, IN1_BR, IN2_BR);
}

void diagonalRight() {
  // servo_fl.write(120 + fl_ofst);
  // servo_ml.write(120 + ml_ofst);
  // servo_bl.write(120 + bl_ofst);
  // servo_fr.write(120 + fr_ofst);
  // servo_mr.write(120 + mr_ofst);
  servo_br.write(120 + br_ofst);

  delay(15);
}

void diagonalLeft() {
  // servo_fl.write(60 + fl_ofst);
  // servo_ml.write(60 + ml_ofst);
  // servo_bl.write(60 + bl_ofst);
  // servo_fr.write(60 + fr_ofst);
  // servo_mr.write(60 + mr_ofst);
  servo_br.write(60 + br_ofst);

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

