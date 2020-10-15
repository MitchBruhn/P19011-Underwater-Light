/*
    Name:       boat_light.ino
    Created:	9/24/2019 8:23:57 AM
    Author:     DESKTOP-T99U9L0\Kent
*/

// pin mapping
#define TEMP_IN		A2
#define PWM_OUT		1

// foldback range in degrees C
#define startTemp	60.0
#define endTemp		80.0

// filter stuff
#define IIR			16
long adcFiltered = -9999;

// foldback range ADC values
unsigned int startADC;
unsigned int endADC;
bool blinkMode;
bool blinkState = false;
unsigned int pwmValue;
int oldPwmValue = -1;

void setup()
{
	pinMode(PWM_OUT, OUTPUT);
	analogWrite(PWM_OUT, 255);

  startADC = (unsigned int)((0.500 + (startTemp * 0.01))/(5.0/1024.0));
  endADC = (unsigned int)((0.500 + (endTemp * 0.01))/(5.0/1024.0));
}


void loop()
{ 
	if (adcFiltered < -9990)
		adcFiltered = analogRead(TEMP_IN);
	else
		adcFiltered = (blinkMode) ?  analogRead(TEMP_IN) : (adcFiltered * (IIR - 1) + analogRead(TEMP_IN)) / IIR;

  pwmValue = constrain(map(adcFiltered, startADC, endADC, 255, 0), 0, 255);

  blinkMode = (adcFiltered >= endADC) ? true : false;
  blinkState = !blinkState;

  if (blinkMode)
  {
    analogWrite(PWM_OUT, (blinkState == true) ? 64 : 0);
    delay((blinkState)? 50: 950);
  }
  else
  {
    if (oldPwmValue < 0)
    {
      oldPwmValue = pwmValue;
      analogWrite(PWM_OUT, oldPwmValue);
    }
    
    while (oldPwmValue < pwmValue)
    {
      oldPwmValue++;
      analogWrite(PWM_OUT, oldPwmValue);
      delay(50);       
    }

    while (oldPwmValue > pwmValue)
    {
      oldPwmValue--;
      analogWrite(PWM_OUT, oldPwmValue);
      delay(50); 
    }
    
    oldPwmValue = pwmValue;
    delay(50);    
  }
}
