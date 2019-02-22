    /*     Simple Stepper Motor Control Exaple Code
     *      
     *  by Dejan Nedelkovski, www.HowToMechatronics.com
     *  
     */
    // defines pins numbers

#ifndef STEPMOTOR_H
#define STEPMOTOR_H



class StepMotor
{
public:
	StepMotor(uint16_t nb_step,uint8_t enaPin, uint8_t stepPin, uint8_t dirPin,uint8_t M0Pin, uint8_t M1Pin);
	void SetDirPolarity(bool Avance);
	void SetMinPulse(uint16_t MinPulse);
	void SetSpeedTreshold(uint32_t SpeedTreshold);
	void move(void);
	void move(int32_t speed);
	void move(int32_t speed, int32_t nbstep);
private:
	bool Avance=true;
	bool Recule=false;

	uint16_t nb_step;
	uint8_t stepPin; 
	uint8_t dirPin;
	uint8_t enaPin;
	uint8_t M0Pin;
	uint8_t M1Pin;
	uint32_t SpeedTreshold;
	uint16_t cnt_stop=0;
	uint32_t temps_prochain; // microseconde
	uint8_t  prochain_etat;
	uint16_t Min_pulse=300; // 300 micro est le pulse le plus petit dans un état haut ou bas
	int32_t vitesse;
	uint32_t SetDiv(uint32_t Speed);
	void Setmstep(uint8_t mstep);
	int32_t _nbstep=0;
	bool _end=false;
	//bool Between(value, treshold);
};

#endif