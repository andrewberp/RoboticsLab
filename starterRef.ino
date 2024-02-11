#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;

Encoders encoders;
Buzzer buzzer;
Motors motors;
ButtonA buttonA;

unsigned long currentMillis;
unsigned long prevMillis;
const unsigned long PERIOD = 20;

long countsLeft = 0;
long countsRight = 0;
long prevLeft = 0;
long prevRight = 0;

const int CLICKS_PER_ROTATION = 12;
const float GEAR_RATIO = 75.81F;
const float WHEEL_DIAMETER = 3.2F;
const int WHEEL_CIRCUMFERENCE = 10.0531;

float Sl = 0.0F;
float Sr = 0.0F;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(1000);
  buzzer.play("c32");

}

void loop() {
  // put your main code here, to run repeatedly:
  checkEncoders();
  }


void checkEncoders() {
  currentMillis = millis();
  if(currentMillis > prevMillis + PERIOD) {
    countsLeft += encoders.getCountsAndResetLeft();
    countsRight += encoders.getCountsAndResetRight();

    Sl += ((countsLeft - prevLeft) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);
    Sr += ((countsLeft - prevRight) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);

    int wheelSpeed = 200;
    if(Sl < 30.5) {
      motors.setSpeeds(wheelSpeed, wheelSpeed);
    }
      if(Sl > 20) {
      wheelSpeed = 100 * ((30 - Sl) / 10);
        if(wheelSpeed < 20) wheelSpeed = 20; {

        }
      motors.setSpeeds(wheelSpeed, wheelSpeed);
    }

    else {
      motors.setSpeeds(0, 0);
    }

    Serial.print("Left: ");
    Serial.print(Sl);
    Serial.print(" Right : ");
    Serial.println(Sr);

    prevMillis = currentMillis;

  
  }


}
