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
  if (currentMillis > prevMillis + PERIOD) {
    countsLeft += encoders.getCountsAndResetLeft();
    countsRight += encoders.getCountsAndResetRight();

    Sl += ((countsLeft - prevLeft) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);
    Sr += ((countsRight - prevRight) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);

    static int movementPhase = 0; // 0: First forward, 1: Backward, 2: Second forward
    int wheelSpeed = 100; // Default wheel speed

    bool hasReachedTarget = false;
    if (movementPhase == 0) {
      // For the first forward movement
      hasReachedTarget = Sl >= 30.5; // Target distance for the first movement (12 inches)
    } else if (movementPhase == 1) {
      // For backward movement, check if the robot has moved the equivalent distance in negative
      hasReachedTarget = Sl <= -30.5; // Target distance for backward movement
    } else if (movementPhase == 2) {
      // For the second forward movement, the target distance should be for 18 inches
      // Convert 18 inches to cm (18 * 2.54 = 45.72 cm), and adjust since Sl is reset at each phase
      hasReachedTarget = Sl >= 45.72; // Target distance for the second movement
    }

    // Apply de-acceleration logic and speed adjustments
    if (!hasReachedTarget) {
      // Adjust de-acceleration logic to account for the specific phase
      float phaseTargetDistance = movementPhase == 2 ? 45.72 : 30.5;
      if ((Sl > phaseTargetDistance - 10 && movementPhase != 1) || (Sl < -(phaseTargetDistance - 10) && movementPhase == 1)) {
        wheelSpeed = 100 * ((phaseTargetDistance - fabs(Sl)) / 10);
        if (wheelSpeed < 20) wheelSpeed = 20;
      }
      if (movementPhase == 1) wheelSpeed = -wheelSpeed; // Invert speed for backward movement
      motors.setSpeeds(wheelSpeed, wheelSpeed);
    } else {
      // Stop the robot and prepare for the next phase
      motors.setSpeeds(0, 0);

      if (movementPhase == 2) {
        // Play a buzz sound to signify the end of the program
        buzzer.play("c32"); // Example tone, adjust as needed
      }


      // Reset distances for the next movement
      Sl = 0;
      Sr = 0;
      countsLeft = 0;
      countsRight = 0;
      prevLeft = 0;
      prevRight = 0;

      // Advance to the next phase
      movementPhase++;
      if (movementPhase > 2) movementPhase = 0; // Loop back to the first phase after completing all movements

      // Small delay to ensure the robot fully stops before the next movement
      delay(200);
    }

    Serial.print("Phase: ");
    Serial.print(movementPhase);
    Serial.print(" Left: ");
    Serial.print(Sl);
    Serial.print(" Right: ");
    Serial.println(Sr);

    prevLeft = countsLeft;
    prevRight = countsRight;
    prevMillis = currentMillis;
  }
}
