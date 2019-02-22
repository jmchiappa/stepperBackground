    /*     Simple Stepper Motor Control Example Code
     *      
     *  by Jean-Marc Chiappa
     *  
     */

    // defines pins numbers
#include "motor.h"

#define stepPinMoteurGauche 3 
#define dirPinMoteurGauche  2 
#define enablePinMoteurGauche  6
#define M0PinMoteurGauche  10
#define M1PinMoteurGauche  11


#define AXE_X   A0
StepMotor MoteurGauche(400,enablePinMoteurGauche,stepPinMoteurGauche, dirPinMoteurGauche,M0PinMoteurGauche,M1PinMoteurGauche);

void setup() {
  MoteurGauche.SetDirPolarity(false); // arbitrary clock around
  MoteurGauche.SetSpeedTreshold(2800);  // limit of slow motion

}

int16_t speed=-2800;
uint32_t t=0;
#define DELAY_MS  10

void loop() {
    if(millis()>(t+DELAY_MS))
    {
      MoteurGauche.move(speed);     // set updated speed
      t=millis();                   // update timeout
      speed +=10;                   // increase the speed and
      if(speed>2900) speed =-2800;  // overrun, modulo treshold
    }
    MoteurGauche.move();  // let's compute the next transition by the object
}
