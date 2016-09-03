/***********************************************************************
   Motor driver function definitions - by James Nugen and Chaoyang Liu
   ***********************************************************************/

  void initMotorController();
  void setMotorSpeed(int i, int spd);
  void setMotorSpeeds(int leftSpeed, int rightSpeed);
  #ifdef L298N_DUAL_HBRIDGE
    // motor one
    #define ENA 5
    #define IN1 7
    #define IN2 8
    // motor two
    #define ENB 6
    #define IN3 9
    #define IN4 10
  #endif
