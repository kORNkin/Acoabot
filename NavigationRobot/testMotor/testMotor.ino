#define ENCA_R 44 // YELLOW
#define ENCB_R 42 // WHITE

#define PWM_BL 2
#define IN1_BL 22
#define IN2_BL 24

#define PWM_B 3
#define IN1_B 26
#define IN2_B 28

void setup() {
  Serial.begin(9600);

  pinMode(ENCA_R,INPUT);
  pinMode(ENCB_R,INPUT);

  pinMode(PWM_BL,OUTPUT);
  pinMode(IN1_BL,OUTPUT);
  pinMode(IN2_BL,OUTPUT);

  pinMode(PWM_B,OUTPUT);
  pinMode(IN1_B,OUTPUT);
  pinMode(IN2_B,OUTPUT);
}

void loop(){
  //testMotor();
  forward();
}

void forward(){
  setMotor(1, 100, PWM_BL, IN1_BL, IN2_BL);

  setMotor(1, 20, PWM_B, IN1_B, IN2_B);
}

void testMotor() {
  setMotor(1, 255, PWM_BL, IN1_BL, IN2_BL);

  delay(1000);

  setMotor(1, 100, PWM_BL, IN1_BL, IN2_BL);

  delay(500);

  setMotor(0, 0, PWM_BL, IN1_BL, IN2_BL);

  delay(1000);

  setMotor(-1, 100, PWM_BL, IN1_BL, IN2_BL);

  delay(500);

  setMotor(-1, 255, PWM_BL, IN1_BL, IN2_BL);

  delay(1000);

  setMotor(0, 0, PWM_BL, IN1_BL, IN2_BL);

  delay(1000);
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

