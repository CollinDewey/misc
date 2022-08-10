#include <Servo.h>
#include <Wire.h>
#include <SparkFunTMP102.h>

#define TEMP_TOLERANCE 1.0
#define TEMP_RANGE 8.0
#define TEMP_BASE 66
#define SERVO_HOME 8
#define SERVO_DEST 33
#define DEBUG 1

enum modes {heat = 1, cool = 0};
enum modes activeMode = cool;
TMP102 tempSensor;
bool switchEnabled = false;
int servoSpeed = 10;
int checkInterval = 2500;

// Class for controlling writing to the servo and swiping it.
class servo {
  private:
    Servo control;
    int pin;
  public:
    servo(int p) {
      pin = p;
    }
    void write(int destination) {
      control.attach(pin);
      swipe(destination);
      control.detach();
    }
    void write(int destination1, int destination2) {
      control.attach(pin);
      swipe(destination1);
      delay(100);
      swipe(destination2);
      control.detach();
    }
    void swipe(int destination) {
      int pos = control.read() / 5;
      while (pos != destination) {
        if (pos > destination)
          control.write(--pos * 5);
        else
          control.write(++pos * 5);
        delay(servoSpeed);
      }
    }
};

servo highServo(14);
servo lowServo(12);

void setup() {
  // Setup Serial
  Serial.begin(9600);

  // Setup Temp Sensor
  Wire.begin(4, 5);
  tempSensor.begin();

  // Attach and Center Servos
  highServo.write(SERVO_HOME);
  lowServo.write(SERVO_HOME);

  // Wait for Servos to get in position
  delay(100);
}

void loop() {
  // Gather data
  float realTemp;
  int potentiometer = analogRead(A0);
  float desiredTemp = (float(potentiometer)/1024.0*TEMP_RANGE)+TEMP_BASE;
  tempSensor.wakeup();
  realTemp = tempSensor.readTempF();
  tempSensor.sleep();

  // Logic
  if (!switchEnabled && (activeMode ? realTemp < (desiredTemp - TEMP_TOLERANCE) : realTemp > (desiredTemp + TEMP_TOLERANCE))) {
    switchEnabled = true;
    lowServo.write(SERVO_DEST, SERVO_HOME);
  }
  if (switchEnabled && (activeMode ? realTemp > (desiredTemp + TEMP_TOLERANCE) : realTemp < (desiredTemp - TEMP_TOLERANCE))) {
    switchEnabled = false;
    highServo.write(SERVO_DEST, SERVO_HOME);
  }

  #ifdef DEBUG
  
  // Serial Debug Test
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    switch(inByte) {
      case 'T': // Test Servos
        lowServo.write(SERVO_DEST, SERVO_HOME);
        delay(50);
      case 'H': // Test High Servo
        highServo.write(SERVO_DEST, SERVO_HOME);
        break;
      case 'L': // Test Low Servo
        lowServo.write(SERVO_DEST, SERVO_HOME);
        break;
      case '+': // Servo Speed Up
        servoSpeed++;
        break;
      case '-': // Servo Speed Down
        servoSpeed--;
        break;
      case 'S': // Switch Heating/Cooling Modes
        activeMode = activeMode ? cool : heat;
        break;
      case 'C': // Home Servo
        lowServo.write(SERVO_HOME);
        highServo.write(SERVO_HOME);
        break;
    }
  }
  
  // Debug info
  char debugString[128];
  sprintf(debugString, "Potentiometer: %d, Temp: %.2f, RealTemp: %.2f, Switch: %d, Speed: %d, Mode: %d", potentiometer, desiredTemp, realTemp, switchEnabled, servoSpeed, activeMode);
  Serial.println(debugString);
  
  #endif
    
  delay(checkInterval);
}
