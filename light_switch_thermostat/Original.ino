#include <Servo.h>
#include <Wire.h>
#include <SparkFunTMP102.h>

Servo highServo;
Servo lowServo;
TMP102 tempSensor;
bool heatEnabled = false;
float tolerance = 1;
float tempRange = 8.0;
int servoSpeed = 3;
int homePosition = 50;
int baseTemp = 66;
int checkInterval = 2500;

void setup() {
  // Setup Serial
  Serial.begin(9600);

  // Setup Temp Sensor
  Wire.begin(4, 5);
  tempSensor.begin();

  // Attach and Center Servos
  highServo.attach(14);
  lowServo.attach(12);
  highServo.write(60);
  lowServo.write(60);

  // Wait for Servos to get in position
  delay(100);
}

void useServo(Servo selectedServo) {
  int pos;

  // Swing to position
  for (pos = homePosition; pos <= 165; pos += servoSpeed) {
    selectedServo.write(pos);
    delay(15);
  }

  // Slowly home
  while (pos > homePosition) {
    selectedServo.write(--pos);
    delay(25);
  }
}

void loop() {
  // Gather data
  float realTemp;
  int potentiometer = analogRead(A0);
  float desiredTemp = (float(potentiometer)/1024.0*tempRange)+baseTemp;
  tempSensor.wakeup();
  realTemp = tempSensor.readTempF();
  tempSensor.sleep();

  // Logic
  if (!heatEnabled && realTemp < (desiredTemp - tolerance)) {
    heatEnabled = true;
    useServo(lowServo);
  }
  if (heatEnabled && realTemp > (desiredTemp + tolerance)) {
    heatEnabled = false;
    useServo(highServo);
  }

  // Test Function
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    switch(inByte) {
      case 'T':
        useServo(lowServo);
        delay(50);
        useServo(highServo);
        delay(50);
        break;
      case 'H':
        useServo(highServo);
        delay(50);
        break;
      case 'L':
        useServo(lowServo);
        delay(50);
        break;
    }
  }

  // Debug info
  Serial.print("Value: ");
  Serial.print(potentiometer);
  Serial.print(" Temp: ");
  Serial.print(desiredTemp);
  Serial.print(" RealTemp: ");
  Serial.print(realTemp);
  Serial.print(" Heat: ");
  Serial.println(heatEnabled);

  delay(checkInterval);
}
