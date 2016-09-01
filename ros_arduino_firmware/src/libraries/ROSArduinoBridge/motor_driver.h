/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/
  const int ENA = 6;
  const int IN1 = 8;
  const int IN2 = 7;
  const int IN3 = 2;
  const int IN4 = 4;
  const int ENB = 3;
  void initMotorController();
  void setMotorSpeed(int i, int spd);
  void setMotorSpeeds(int leftSpeed, int rightSpeed);
