/*
	
*/
#ifndef SoundInput_h
#define SoundInput_h

#include "Arduino.h"


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
		void begin();	
	private:
	
};



#endif