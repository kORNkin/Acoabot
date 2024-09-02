#define ENCA_R 19 // YELLOW
#define ENCB_R 18 // WHITE

#define PWM_BL 14
#define IN1_BL 12
#define IN2_BL 13

void setup() {
  Serial.begin(9600);

  pinMode(ENCA_R,INPUT);
  pinMode(ENCB_R,INPUT);

  pinMode(PWM_BL,OUTPUT);
  pinMode(IN1_BL,OUTPUT);
  pinMode(IN2_BL,OUTPUT);
}

void loop(){
  forward();
}

void forward() {
  setMotor(1, 255, PWM_BL, IN1_BL, IN2_BL);
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

