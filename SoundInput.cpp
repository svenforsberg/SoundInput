#include "Arduino.h"
#define SoundInput_cpp
#include "SoundInput.h"

extern void (*_isr_func)(int);
SoundInput::SoundInput(void (*function)(int))
{
	_isr_func=function;
}

void SoundInput::begin(int adref)
{
  /*
  ADC_CT=16M/PSC=Fs*13
   40 kHz: 40*13=520 kHz
   10 kHz: 10*13=130 kHz
   PSC(10k)=16M/Fs/13=123
   
   */
  cli();//disable interrupts
  //set up continuous sampling of analog pin 0
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;

  /*
  Reference voltage options: (REFS1 REFS0)
   AREF - external voltage (0 0)=0
   AVCC pin - 5V (0 1)=1
   Internal 1.1V (1 1)=3
   */
  switch(adref)
  {
	case 0: //AREF
		ADMUX &= ~(1 << REFS0); 
		ADMUX &= ~(1 << REFS1); 
		break;
	case 1: //AVCC pin (5V)
		ADMUX |= (1 << REFS0);
		ADMUX &= ~(1 << REFS1);
		break;
	case 3: //Internal reference
		ADMUX |= (1 << REFS0); 
		ADMUX |= (1 << REFS1); 
		break;	
	default: //AREF
		ADMUX &= ~(1 << REFS0); 
		ADMUX &= ~(1 << REFS1);
		break;
  }
  //ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADMUX &= ~(1 << ADLAR); //right align the ADC value- so we can read lowest 8 bits from ADCL register

  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16MHz/32=500kHz
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //set ADC clock with 128 prescaler- 16MHz/128/13=9615 ksps
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements

  sei();//enable interrupts
}
