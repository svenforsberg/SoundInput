#include "SoundInput.h"

/*
Sample program to print incoming ADC samples to serial port!
*/

int buf[100],buf_ct=0,lock=0;
SoundInput Sound=SoundInput(&putBuffer);

void setup()
{
  Sound.begin(SoundInput_AREF);
  Serial.begin(115200);
}

void putBuffer(int val)
{
  if(lock==0){
  buf[buf_ct]=val;
  buf_ct++;
  if(buf_ct>100) buf_ct=0;
  }
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
  }
}



