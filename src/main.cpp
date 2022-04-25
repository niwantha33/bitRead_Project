#include <Arduino.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <binary.h>

/*

  ---------       -------
  |       |       |       |
  |  SB   |   (1) |  (2)  |   (3)     (4)
  |       |       |       |
--|       |------ |       |------------------------

SB =  (START BIT)

*/

uint8_t input_pin_x = 5;
uint8_t input_pin_y = 8;

uint8_t output_pin = 6;

uint8_t enable_pin = 7;

int flagX = 0, flagY = 0;

void read_x()
{
  flagX = 1;
}

void read_y()
{
  flagY = 1;
}
void setup()
{
  pinMode(input_pin_x, INPUT_PULLDOWN);
  pinMode(input_pin_y, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(input_pin_x), read_x, FALLING);
  attachInterrupt(digitalPinToInterrupt(input_pin_y), read_y, FALLING);
}

void loop()
{
  int i = 0;
  if (flagX)
  {
    detachInterrupt(input_pin_x);
    uint8_t x[4] ={'\0'}; 
    for(i=4; i>0; i--)
    {
      x[i] = digitalRead(input_pin_x);
      delayNanoseconds(100);

    }
    flagX = 0;
  }
    if (flagY)
  {
    detachInterrupt(input_pin_y);
    flagY = 0;
  }
}