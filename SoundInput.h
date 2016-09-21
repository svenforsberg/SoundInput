/*
	
*/
#ifndef SoundInput_h
#define SoundInput_h

#include "Arduino.h"

#define SoundInput_AREF 0
#define SoundInput_AVCC 1
#define SoundInput_INTERNAL 3

#ifndef SoundInput_cpp //Needed in application
int _adc_data;
void (*_isr_func)(int);

	ISR(ADC_vect) 
	{
		_adc_data = ADCW;
		_adc_data-=512;
		_isr_func(_adc_data);
	}
#endif


class SoundInput //Needed in SoundInput.cpp
{
	public:
		SoundInput( void (*function)(int) );
		void begin(int);	
	private:
	
};



#endif