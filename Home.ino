#include <AccelStepper.h>

// Define the stepper motor pins
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Define the homing speed and direction
#define HOMING_SPEED 10
#define HOMING_DIRECTION -1

// Define the degrees per step
#define DEGREES_PER_STEP 0.0879 // 360 / 4096

// Create the stepper motor object
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN2, IN3, IN4);

// Define the home position
long homePosition = 0;

void setup() {
  // Set the maximum speed and acceleration of the stepper motor
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(50);

  // Move the motor to the home position
  moveMotorToHome();
}

void loop() {
  // Move the motor 50 degrees clockwise and then back to the home position
  moveMotorToPosition(homePosition + 50);
  moveMotorToHome();

  // Move the motor 50 degrees anticlockwise and then back to the home position
  moveMotorToPosition(homePosition - 50);
  moveMotorToHome();
}

// Function to move the motor to a specified position
void moveMotorToPosition(long position) {
  // Calculate the number of steps to move the motor
  long stepsToMove = (position - stepper.currentPosition()) / DEGREES_PER_STEP;

  // Set the motor speed and direction
  stepper.setSpeed(50);
  if (stepsToMove > 0) {
    stepper.setDirection(1);
  } else {
    stepper.setDirection(-1);
    stepsToMove = -stepsToMove;
  }

  // Move the motor to the specified position
  stepper.move(stepsToMove);
  while (stepper.distanceToGo() != 0) {
    stepper.runSpeedToPosition();
  }
}

// Function to move the motor to the home position
void moveMotorToHome() {
  // Move the motor in the homing direction at the homing speed
  stepper.setSpeed(HOMING_SPEED * HOMING_DIRECTION);
  while (!digitalRead(END_STOP_PIN)) {
    stepper.runSpeed();
  }

  // Stop the motor and set the current position as the home position
  stepper.stop();
  homePosition = stepper.currentPosition();
}
