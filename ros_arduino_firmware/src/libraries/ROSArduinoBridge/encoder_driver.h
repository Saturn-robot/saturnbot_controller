/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */


#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins;
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN_A PD2  //pin 2
  #define LEFT_ENC_PIN_B PD3  //pin 3

  //below can be changed, but should be PORTC pins
  #define RIGHT_ENC_PIN_A PC4  //pin A4
  #define RIGHT_ENC_PIN_B PC5   //pin A5
#endif

#ifdef ARDUINO_MY_COUNTER
  #define LEFT_ENC_A 2
  #define LEFT_ENC_B 22
  #define RIGHT_ENC_A 21
  #define RIGHT_ENC_B 24
  void initEncoders();
  void leftEncoderEvent();
  void rightEncoderEvent();
#endif

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
