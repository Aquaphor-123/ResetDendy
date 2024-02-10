#include <Arduino.h>
#include "NESDriver.h"

#define DATA0_PIN 8
#define LATCH_PIN 7
#define CLOCK_PIN 4
#define RESET 5
#define BUTTON 9

NESController c;

void setup() 
{
    NESDriver.begin(LATCH_PIN, CLOCK_PIN, DATA0_PIN);
    pinMode(RESET,INPUT_PULLUP);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() 
{
  NESDriver.updateState();
  c = NESDriver.state1();
  if (c.isChanged()) 
  {  
    Serial.println(c.state(), HEX);
    if (c.start() && c.select()) 
    {
      Serial.println('R');
    } 
  }

  if (!digitalRead(RESET)) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1)
    {
      if (digitalRead(RESET))
      {
        if (digitalRead(BUTTON)) 
        {
          Serial.println();
          break;
        }          
      } else 
      {
        Serial.print('!');
        delay(100);           
      }  
    }
    digitalWrite(LED_BUILTIN, LOW);           
    } 
  delay(10);
}
