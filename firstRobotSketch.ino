#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;

Encoders encoders;
Buzzer buzzer;
Motors motors;
ButtonA buttonA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(1000);
  buzzer.play("c32");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(buttonA.isPressed()) {
    motors.setSpeeds(100, 100);
    Serial.println("Motors running");
  }
  else {
    motors.setSpeeds(0, 0);
  }

}

