    /*     mobile plaform Stepper Motor Control Example Code
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

#define stepPinMoteurDroite  5
#define dirPinMoteurDroite  4 
#define enablePinMoteurDroite  7
#define M0PinMoteurDroite  8
#define M1PinMoteurDroite  9

// X and Y axes of the joystick are respectively connected to A0 and A1

#define AXE_X   A0
#define AXE_Y   A1
StepMotor MoteurGauche(400,enablePinMoteurGauche,stepPinMoteurGauche, dirPinMoteurGauche,M0PinMoteurGauche,M1PinMoteurGauche);
StepMotor MoteurDroite(400,enablePinMoteurDroite,stepPinMoteurDroite, dirPinMoteurDroite,M0PinMoteurDroite,M1PinMoteurDroite);

uint32_t x0_offset;
uint32_t y0_offset;

inline int8_t sign(int32_t x){return (x<0)?-1:+1;};

void setup() {

  MoteurDroite.SetDirPolarity(true);  //un moteur dans un sens...
  MoteurGauche.SetDirPolarity(false); /// l'autre dans l'autre
  MoteurDroite.SetSpeedTreshold(2800);  // temps minimum de prise en compte, au-dessus le moteur est à l'arrêt
  MoteurGauche.SetSpeedTreshold(2800);

  Serial.begin(115200);
  // Sets  pins as Outputs

  x0_offset = analogRead(AXE_X);
  y0_offset = analogRead(AXE_Y);

  // Serial.println("démarrage...");

}
void loop() {
    int32_t x= analogRead(AXE_X)-x0_offset;
    int32_t y= analogRead(AXE_Y)-y0_offset;
    int32_t vg = map(abs(y+x),0,512,3000,300)*sign(y+x);
    int32_t vd = map(abs(y-x),0,512,3000,300)*sign(y-x);

    MoteurDroite.move(vd);
    MoteurGauche.move(vg);

}
