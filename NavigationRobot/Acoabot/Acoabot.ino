//#include <util/atomic.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x08
#include <Servo.h>

#include <TinyGPS++.h>

TinyGPSPlus gps; // create gps object


// ///Encoder motor
// //R
// #define ENCB_RF 22 // WHITE
// #define ENCA_RF 24 // YELLOW

// #define ENCA_RM 26 
// #define ENCB_RM 28 

// #define ENCA_RB 30 
// #define ENCB_RB 32
// //L
// #define ENCA_LF 34
// #define ENCB_LF 36 

// #define ENCA_LM 38 
// #define ENCB_LM 40

// #define ENCA_LB 42
// #define ENCB_LB 44

///Motor dirver
//R
#define PWM_RF 13
#define IN1_RF 25
#define IN2_RF 23

// #define PWM_RM 12
// #define IN1_RM 27
// #define IN2_RM 29

#define PWM_RB 12
#define IN1_RB 31
#define IN2_RB 33
//L
#define PWM_LF 11
#define IN1_LF 35
#define IN2_LF 37

// #define PWM_LM 9
// #define IN1_LM 39
// #define IN2_LM 41

#define PWM_LB 10
#define IN1_LB 45
#define IN2_LB 43

#define relayPin 51
#define lightPin 53
bool light = 0;

//Define servos
Servo servo_RF;
Servo servo_RM;
Servo servo_RB;

Servo servo_LF;
Servo servo_LM;
Servo servo_LB;

Servo camL;
Servo camH;

/*
FL 102
ML 95
BL 75
*/

//define servo offset values
int RF_ofst = 4;
int RM_ofst = 0;
int RB_ofst = -15;

int LF_ofst = -14;
int LM_ofst = -14;
int LB_ofst = -3;

//define servo starting position
int start_angle = 90;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(57600); // connect serial
  Serial3.begin(9600); // connect gps sensor

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(relayPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(relayPin, 1);  
  digitalWrite(lightPin, 0); 
  light = 0;

  // pinMode(ENCA_RF,INPUT);
  // pinMode(ENCB_RF,INPUT);

  // pinMode(ENCA_RM,INPUT);
  // pinMode(ENCB_RM,INPUT);

  // pinMode(ENCA_RB,INPUT);
  // pinMode(ENCB_RB,INPUT);

  // pinMode(ENCA_LF,INPUT);
  // pinMode(ENCB_LF,INPUT);

  // pinMode(ENCA_LM,INPUT);
  // pinMode(ENCB_LM,INPUT);

  // pinMode(ENCA_LB,INPUT);
  // pinMode(ENCB_LB,INPUT);

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

  camL.attach(9);
  camH.attach(8);

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
    else if(inputData.startsWith("w")){ forward();
    }else if(inputData.startsWith("s")){ reverse();
    }else if(inputData.startsWith("a")){ left();
    }else if(inputData.startsWith("x")){ straight();
    }else if(inputData.startsWith("d")){ right();
    }else if(inputData.startsWith("r")){ calibrate();
    }else if(inputData.startsWith("z")){ slide();
    }else if(inputData.startsWith("q")){ turnLeft();
    }else if(inputData.startsWith("e")){ turnRight();
    }else if(inputData.startsWith("h")){ stopMotor();
    }else if(inputData.startsWith("o")){ camUp();
    }else if(inputData.startsWith("l")){ camDown();
    }else if(inputData.startsWith("f")){
      if(light){
        digitalWrite(lightPin, 0);
      }else {
        digitalWrite(lightPin, 1);
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

  while(Serial3.available()){ // check for gps data
    if(gps.encode(Serial3.read()))// encode gps data
    {
      Serial.print("GPS:");
      Serial.print(gps.location.lat(),6); //Latitude
      Serial.print(", ");
      Serial.println(gps.location.lng(),6); //Longitude

      // //Altitude
      // Serial.print("Altitude: "); 
      // Serial.println(gps.altitude.feet());

      // //Speed
      // Serial.print("Speed: "); 
      // Serial.println(gps.speed.mph());

      // // Number of satellites connected
      // Serial.print("Number of Sats connected: "); 
      // Serial.println(gps.satellites.value());

      delay(10);

    }
  }
}

void forward() {
  setMotor(-1, 255, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 255, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 255, PWM_RB, IN1_RB, IN2_RB);

  setMotor(-1, 255, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 255, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 255, PWM_LB, IN1_LB, IN2_LB);
}

void reverse() {
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
  servo_RF.write(130 + RF_ofst);
  servo_RM.write(110 + RM_ofst);
  servo_RB.write(90 + RB_ofst);

  servo_LF.write(130 + LF_ofst);
  servo_LM.write(110 + LM_ofst);
  servo_LB.write(90 + LB_ofst);
} 

void left(){
  servo_RF.write(50 + RF_ofst);
  servo_RM.write(70 + RM_ofst);
  servo_RB.write(90 + RB_ofst);

  servo_LF.write(50 + LF_ofst);
  servo_LM.write(70 + LM_ofst);
  servo_LB.write(90 + LB_ofst);
} 

void turnRight(){
  servo_RF.write(60 + RF_ofst);
  servo_RM.write(start_angle + RM_ofst);
  servo_RB.write(120 + RB_ofst);

  servo_LF.write(120 + LF_ofst);
  servo_LM.write(start_angle + LM_ofst);
  servo_LB.write(60 + LB_ofst);

  delay(1000);

  setMotor(1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(1, 200, PWM_RB, IN1_RB, IN2_RB);

  setMotor(-1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 200, PWM_LB, IN1_LB, IN2_LB);
}

void turnLeft(){
  servo_RF.write(60 + RF_ofst);
  servo_RM.write(start_angle + RM_ofst);
  servo_RB.write(120 + RB_ofst);

  servo_LF.write(120 + LF_ofst);
  servo_LM.write(start_angle + LM_ofst);
  servo_LB.write(60 + LB_ofst);

  delay(1000);

  setMotor(-1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 200, PWM_RB, IN1_RB, IN2_RB);

  setMotor(1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(1, 200, PWM_LB, IN1_LB, IN2_LB);
}

void smallRight(){
  servo_RF.write(105 + RF_ofst);
  servo_RM.write(95 + RM_ofst);
  servo_RB.write(90 + RB_ofst);

  servo_LF.write(105 + LF_ofst);
  servo_LM.write(95 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void smallLeft(){
  servo_RF.write(75 + RF_ofst);
  servo_RM.write(85 + RM_ofst);
  servo_RB.write(90 + RB_ofst);

  servo_LF.write(75 + LF_ofst);
  servo_LM.write(85 + LM_ofst);
  servo_LB.write(90 + LB_ofst);

  delay(15);
}

void calibrate(){
  setMotor(1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(1, 200, PWM_RB, IN1_RB, IN2_RB);
  setMotor(1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(1, 200, PWM_LB, IN1_LB, IN2_LB);

  delay(400);

  setMotor(-1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 200, PWM_RB, IN1_RB, IN2_RB);
  setMotor(-1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 200, PWM_LB, IN1_LB, IN2_LB);

  delay(400);

  setMotor(1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(1, 200, PWM_RB, IN1_RB, IN2_RB);
  setMotor(1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(1, 200, PWM_LB, IN1_LB, IN2_LB);

  delay(400);

  setMotor(-1, 200, PWM_RF, IN1_RF, IN2_RF);
  setMotor(-1, 200, PWM_RM, IN1_RM, IN2_RM);
  setMotor(-1, 200, PWM_RB, IN1_RB, IN2_RB);
  setMotor(-1, 200, PWM_LF, IN1_LF, IN2_LF);
  setMotor(-1, 200, PWM_LM, IN1_LM, IN2_LM);
  setMotor(-1, 200, PWM_LB, IN1_LB, IN2_LB);

  delay(400);

  setMotor(0, 0, PWM_RF, IN1_RF, IN2_RF);
  setMotor(0, 0, PWM_RM, IN1_RM, IN2_RM);
  setMotor(0, 0, PWM_RB, IN1_RB, IN2_RB);
  setMotor(0, 0, PWM_LF, IN1_LF, IN2_LF);
  setMotor(0, 0, PWM_LM, IN1_LM, IN2_LM);
  setMotor(0, 0, PWM_LB, IN1_LB, IN2_LB); 
}

void slide(){
  servo_RF.write(175 + RF_ofst);
  servo_RM.write(175 + RM_ofst);
  servo_RB.write(175 + RB_ofst);

  servo_LF.write(175 + LF_ofst);
  servo_LM.write(175 + LM_ofst);
  servo_LB.write(175 + LB_ofst);
}

void stopMotor() {
  setMotor(0, 0, PWM_RF, IN1_RF, IN2_RF);
  setMotor(0, 0, PWM_RM, IN1_RM, IN2_RM);
  setMotor(0, 0, PWM_RB, IN1_RB, IN2_RB);

  setMotor(0, 0, PWM_LF, IN1_LF, IN2_LF);
  setMotor(0, 0, PWM_LM, IN1_LM, IN2_LM);
  setMotor(0, 0, PWM_LB, IN1_LB, IN2_LB);
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

void camUp(){
  float l = camL.read();
  float h = camH.read();
  camL.write(min(160, l + 15));
  camL.write(min(160, h + 15));
}

void camDown(){
  float l = camL.read();
  float h = camH.read();
  camL.write(max(0, l - 15));
  camL.write(max(0, h - 15));
}

