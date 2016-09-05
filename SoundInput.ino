void SoundInit()
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
  Reference voltage options:
   AREF - external voltage
   AVCC pin - 5V
   Internal 2.56V
   */
  ADMUX |= (1 << REFS0); //set reference voltage to AVcc
  //ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADMUX &= ~(1 << ADLAR); //right align the ADC value- so we can read lowest 8 bits from ADCL register

  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //set ADC clock with 128 prescaler- 16MHz/128/13=9615 ksps
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements

  sei();//enable interrupts
}


#define OFFSET 512
int adc_data,new_data=0;
unsigned long dT,sampletime;
int buf[100],buf_ct=0,lock=0;

void setup()
{

  SoundInit();
  Serial.begin(115200);
}

void putBuffer(int val)
{
  buf[buf_ct]=val;
  buf_ct++;
  if(buf_ct>100) buf_ct=0;
}

ISR(ADC_vect) 
{
  adc_data = ADCW;
  adc_data-=OFFSET;
  new_data=1;
  if(lock==0) putBuffer(adc_data);
}

void loop()
{
  if(buf_ct==99)
  {
    int i;
    lock=1;
    for(i=0;i<100;i++)
    {
      Serial.println(buf[i]);
    }
    lock=0;
    //Shall be 20 samples for one 500Hz period. Ts=100us.
    //Measured Ts=96 us. (Fs=10417 Hz)

  
  }
}



