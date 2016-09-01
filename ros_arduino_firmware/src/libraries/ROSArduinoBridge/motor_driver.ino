/***************************************************************
   Motor driver definitions

   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.

   *************************************************************/

#ifdef USE_BASE

#ifdef POLOLU_VNH5019
  /* Include the Pololu library */
  #include "DualVNH5019MotorShield.h"

  /* Create the motor driver object */
  DualVNH5019MotorShield drive;

  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined POLOLU_MC33926
  /* Include the Pololu library */
  #include "DualMC33926MotorShield.h"

  /* Create the motor driver object */
  DualMC33926MotorShield drive;

  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }

#elif defined L298N_DUAL_HBRIDGE
// motor one
int enA = 5;
int in1 = 7;
int in2 = 8;
// motor two
int enB = 6;
int in3 = 9;
int in4 = 10;
boolean directionLeft = false;
boolean directionRight = false;

boolean direction(int i){
   if(i == LEFT){
      return directionLeft;
   }else{
      return directionRight;
   }
}
  void initMotorController() {
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  }

  void setMotorSpeed(int i, int spd) {
    if(spd>MAX_PWM){
      spd=MAX_PWM;
    }
       if(spd<-MAX_PWM){
      spd=-1*MAX_PWM;
    }
    if (i == LEFT){
        if(spd>=0){
            directionLeft = FORWARDS;
            digitalWrite(in2, HIGH);
            digitalWrite(in1, LOW);
            analogWrite(enA, spd);
        }else if(spd < 0){
            directionLeft = BACKWARDS;
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            analogWrite(enA, -spd);
        }
    }
    else {
        if(spd>=0){
            directionRight = FORWARDS;
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);
            analogWrite(enB, spd);
        }else if(spd<0){
            directionRight = BACKWARDS;
            digitalWrite(in4, HIGH);
            digitalWrite(in3, LOW);
            analogWrite(enB, -spd);
        }
    }
  }

  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#else
  #error A motor driver must be selected!
#endif

#endif
