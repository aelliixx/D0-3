#include <SevSeg.h>

#include "SevSeg.h"
SevSeg sevseg; 


unsigned long startTime = 0;

float potA;
float timer = 60;


void setup()
{
  //Serial.begin(9600);
  byte numDigits = 4;
  byte digitPins[] = {19, 4, 3, 5};
  byte segmentPins[] = {18, 10, 2, 7, 8, 17, 9, 6};

  pinMode(11, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  pinMode(21, INPUT);
  
  potA = analogRead(21);
  
  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = false;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelaysIn);
  sevseg.setBrightness(100);
}

float roundFloat(float var, int precision)
{
  int decimal = pow(10, precision);
  float value = (int)(var * decimal + .5);
  return (float)value / decimal;
}

void loop()
{  
 int now = millis(); 
 
 if (millis() - startTime >= 10 && timer > 0)
 {

   timer-=.005; 
   startTime = millis();
 }

 if (digitalRead(11))
 {
   sevseg.setNumberF((timer < 30 ? timer * 2 : timer / 30), 2);  
   sevseg.refreshDisplay();
 }

 int readings = 30;
 if (!digitalRead(11))
 {
   potA = 0;
   for (int o = 0; o < readings; o++)
   {
     potA += analogRead(21);
   }
   float val = roundFloat((potA/readings)/(1023) * (60), 1);
   timer = val;
   //Serial.println(potA/readings);
   sevseg.setNumberF((val < 30 ? val * 2 : val / 30), 2);  
   sevseg.refreshDisplay();
 }

 if (timer <= 0 && digitalRead(11))
 {
   timer = 0;
   digitalWrite(12, HIGH);
 }
 else
 {
   digitalWrite(12, LOW);    
 }

}