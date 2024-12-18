#include<ESP32Servo.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h> // For atan2() and sqrt()

Adafruit_MPU6050 mpu;

Servo roll;
Servo pitch;

String inputData;
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  roll.attach(13);
  pitch.attach(12);
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
    if(inputData.startsWith("b")){
      stabilize();
    } else if(inputData.startsWith("n")){
            if(inputData.startsWith("w")){ up();
      }else if(inputData.startsWith("a")){ left();
      }else if(inputData.startsWith("s")){ right();
      }else if(inputData.startsWith("d")){ down();
      }
    }
  }

  delay(10);

  if(Serial.available() > 0) readControl();
}

void stabilize(){
  double start_r = 0.0, start_p = 0.0;

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Calculate Roll and Pitch */
  float rollRead = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180.0 / M_PI;
  float pitchRead = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / M_PI;

  // rollRead = map(rollRead, -90, 90, 0, 180);
  // pitchRead = map(pitchRead, -90, 90, 0, 180);

  delay(10);

  // if(start_r < rollRead - 1 && start_r > rollRead + 1){
  //   roll.write();
  // }

  // if(start_p < pitchRead - 1 && start_p > pitchRead + 1){
  //   pitch.write();
  // }
}

void up(){
  int r = roll.read();
  int p = pitch.read();

 // roll.write(min(160, r + 40));
  pitch.write(min(160, p + 40));
}

void left(){
  int r = roll.read();
  int p = pitch.read();

  roll.write(min(160, r + 40));
  //pitch.write(min(160, p + 40));
}

void right(){
  int r = roll.read();
  int p = pitch.read();

  roll.write(min(160, r - 40));
  //pitch.write(min(160, p - 40));
}

void down(){
  int r = roll.read();
  int p = pitch.read();

  //roll.write(min(160, r - 40));
  pitch.write(min(160, p - 40));
}
