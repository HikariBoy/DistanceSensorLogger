#include <Wire.h>
#include <Adafruit_ADS1015.h>
 
//Adafruit_ADS1015 ads1015;
Adafruit_ADS1115 ads1115(0x48);	// construct an ads1115 at address 0x49
int M;
float distance0;
//float scale;
#define scalexxx (0.983713*0.98931*0.92646*(5/3.3)*(6.144/pow(2,15))*3852.86*1/(1.322*1.2177*1.062))
#define scale 0.5772604056
void setup(void)
{
    float voltage;
  int16_t adc0, adc1, adc2, adc3;
  unsigned long total;

 
  Serial.begin(9600);
  Serial.println("Hello!");
  analogReference(INTERNAL);  // SETS AREF to 1.1V as a reference voltage
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  ads1115.begin();

  total=0;
    M=20;
 for (int i=0;i<M;i++) {
  adc0 = ads1115.readADC_SingleEnded(0);
  total=((unsigned long) adc0) +total;
 }
 //scale=(6.144/pow(2,15))*467e-6;
// scale=(6.144/pow(2,15))*3852.86*1.011/(1.322*1.2177*1.062);
 voltage=((float) total/M)*scale;
 distance0= (((float) total)/M)*scale;
 
}
 
void loop(void)
{
  float distance,voltage,rawADC;
  int16_t adc0, adc1, adc2, adc3;
  unsigned long total;
  total=0;
 for (int i=0;i<M;i++) {
  adc0 = ads1115.readADC_SingleEnded(0);
  total=((unsigned long) adc0) +total;
 }
 //scale=(6.144/pow(2,15))*467e-6;
// scale=(6.144/pow(2,15))*3852.86*1.011/(1.322*1.2177*1.062);
 rawADC=((float) total/M);
 voltage=rawADC*scale;
 distance= rawADC*scale;
  //adc1 = ads1115.readADC_SingleEnded(1);
  //adc2 = ads1115.readADC_SingleEnded(2);
  //adc3 = ads1115.readADC_SingleEnded(3);
  //Serial.print("AIN0: "); 
  Serial.print(rawADC,2);
  Serial.print(",");
  //if( distance>11100 | distance<50)
  if( rawADC>17400 | rawADC<70)
  Serial.println("#Out of Range");
  else
  Serial.println(distance-distance0,1);
  //Serial.print("AIN1: "); Serial.println(adc1);
  //Serial.print("AIN2: "); Serial.println(adc2);
  //Serial.print("AIN3: "); Serial.println(adc3);
  //Serial.println(" ");
  
  delay(1);
}
