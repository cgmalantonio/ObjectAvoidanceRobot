#include <AFMotor.h>

AF_DCMotor motorL(2, MOTOR12_64KHZ); //left wheel
AF_DCMotor motorR(1, MOTOR12_64KHZ); //right wheel

///////All physical movements of the DC motors are handled here///////////////
//All variables are strictly for my unique motors/setup, i doubt it'll work //
//the same way on your setup so some tweaking will be needed here           //
//////////////////////////////////////////////////////////////////////////////

void TurnAround(){
  motorL.setSpeed(180);
  motorR.setSpeed(180);
  motorL.run(FORWARD);
  motorR.run(BACKWARD);
  delay(800);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void Forward(){
  motorL.setSpeed(170);
  motorR.setSpeed(190);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
  delay(1200);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void TurnRight(){
  motorL.setSpeed(180);
  motorR.setSpeed(180);
  motorL.run(FORWARD);
  motorR.run(BACKWARD);
  delay(450);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void TurnLeft(){
  motorL.setSpeed(180);
  motorR.setSpeed(190);
  motorL.run(BACKWARD);
  motorR.run(FORWARD);
  delay(450);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void SlightRight(){
  motorL.setSpeed(180);
  motorR.setSpeed(180);
  motorL.run(FORWARD);
  motorR.run(BACKWARD);
  delay(425);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}


void SlightLeft(){
  motorL.setSpeed(180);
  motorR.setSpeed(180);
  motorL.run(BACKWARD);
  motorR.run(FORWARD);
  delay(425);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void Backward(){
  motorL.setSpeed(120);
  motorR.setSpeed(120);
  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
  delay(1300);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}



