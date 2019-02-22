    /*     Simple Stepper Motor Control Exaple Code
     *      
     *  by Dejan Nedelkovski, www.HowToMechatronics.com
     *  
     */
    // defines pins numbers

#include "Arduino.h"
#include "motor.h"


StepMotor::StepMotor(uint16_t nb_step,uint8_t enaPin, uint8_t stepPin, uint8_t dirPin,uint8_t M0Pin, uint8_t M1Pin)
{
	this->nb_step = nb_step;
	this->enaPin  = enaPin;
	this->stepPin = stepPin;
	this->M0Pin   = M0Pin;
	this->M1Pin   = M1Pin;
	this->dirPin  = dirPin;

	pinMode(this->enaPin,OUTPUT);
	pinMode(this->dirPin,OUTPUT);
	pinMode(this->stepPin,OUTPUT);
	pinMode(this->M0Pin,OUTPUT);
	pinMode(this->M1Pin,OUTPUT);

	digitalWrite(this->enaPin,HIGH); // désactive le driver par défaut
	// La résolution est FULL 1 pas = 1 step
	digitalWrite(this->M0Pin,LOW); 
	digitalWrite(this->M1Pin,LOW);
}

void StepMotor::SetDirPolarity(bool Avance)
{
	if(Avance==true) this->Avance = HIGH; else this->Avance = LOW;
	this->Recule = !(this->Avance)&0x01;
}

void StepMotor::SetMinPulse(uint16_t MinPulse)
{
	this->Min_pulse = MinPulse;
}

void StepMotor::SetSpeedTreshold(uint32_t SpeedTreshold)
{
	this->SpeedTreshold = SpeedTreshold;
}

// Brief : Set speed and compute the microstep divider
// Input : 
//			speed signed integer 
//			the lowest the value is , the highest the speed is
//			For now, "speed" refers to the duration of high or low level of STEP signal in us
//			For example : Speed=1000 means Ton = 1ms and Toff = 1ms. FREQ_STEP=1/(1+1)*10e3 = 500 Hz
//						  For N=200 steps/turn, F = ~2 turns/s = ~120 turns/min
//			
//			Min value = 300 us

void StepMotor::move(int32_t speed)
{
  uint32_t direction;


  if(abs(speed)<this->SpeedTreshold)
	{
		(speed>0) ? direction=Avance:direction=Recule;
		digitalWrite(dirPin,direction);
		this->vitesse = SetDiv(abs(speed));
	}
	else
		this->vitesse=0;
	move();
}

// brief : execute the state machine regarding the speed and nbstep

void StepMotor::move(void)
{
    uint32_t temps_courant_us = micros();
	if(this->vitesse!=0)
	{
	  if(this->temps_prochain<temps_courant_us)
	  {
	    digitalWrite(enaPin,LOW); // démarre le moteur 

	    temps_prochain = temps_courant_us + vitesse;

	    digitalWrite(stepPin,prochain_etat);

	    prochain_etat==HIGH ? prochain_etat=LOW:prochain_etat=HIGH;
	    cnt_stop=0;
	    if(!_end)
	    {
	    	--_nbstep;
	    	if(_nbstep==0) _end=true;
	    }
	  }
  	}
  	else
	  if(cnt_stop<500)
	    cnt_stop++;
	  else
	  {
	    digitalWrite(enaPin,HIGH); 
	    temps_prochain=temps_courant_us;
	  }
}

void StepMotor::move(int32_t speed, int32_t nbstep)
{
	if(nbstep!=0)
	{
		// _nbstep refers to the number of transition HIGH/LOW,
		// so multiply by 2 the number of steps
		_nbstep = abs(nbstep)*2;	
		_end=false;
		if(nbstep<0) speed=-speed;
		// intialize the ustep divider and some stuff
		move(speed);
		do
		{
			move();
		}
		while(!_end);
	}
    digitalWrite(enaPin,HIGH); // Arrête le moteur 
}

// brief : compute the divider related to the speed
// the aim is to set speed always under 900 us. The torque is then optimized

uint32_t StepMotor::SetDiv(uint32_t speed)
{
  byte diviseur=0;
  while(speed>900)
  {
    diviseur++;  // incrémente le diviseur
    speed>>=1; // multiplie par 2 la vitesse
  }
  speed=max((int32_t)speed,(int32_t)Min_pulse); // le pulse minimum est 300µs
  _nbstep<<=diviseur;
  Setmstep(diviseur);
  return speed;
}

// brief : apply ustep value to M[1:0] as of today
// refer to datasheet DRV8825 
// 0 : FULL SCALE
// 1 : 1/2 step
// 2 : 1/4 step
// 3 : 8 microsteps/step
//
// TODO : M2 is currently not driven

void StepMotor::Setmstep(byte ustep)
{
  if((ustep & 0x01)==0x01)
    digitalWrite(M0Pin,HIGH);
  else
    digitalWrite(M0Pin,LOW);
  if((ustep & 0x02)==0x02 )
    digitalWrite(M1Pin,HIGH);
  else
    digitalWrite(M1Pin,LOW);
}

