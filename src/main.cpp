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

uint8_t flagX = 0, flagY = 0, flag_ex = 0, flag_ey = 0, out_y = 0, out_x = 0;
uint8_t y[4] = {'\0'};
uint8_t x[4] = {'\0'};

uint8_t user_a = 12;
uint8_t user_b = 13;
uint8_t user_z = 14;

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
  pinMode(output_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(input_pin_x), read_x, FALLING);
  attachInterrupt(digitalPinToInterrupt(input_pin_y), read_y, FALLING);
}

void loop()
{
  int i = 0;
  if (flagX)
  {
    detachInterrupt(input_pin_x);
    memset(x, 0, sizeof(x));
    for (i = 4; i > 0; i--)
    {
      x[i] = digitalRead(input_pin_x);
      delayNanoseconds(100); /* Need to check with the input */
    }
    flagX = 0;
    flag_ex = 1;
  }
  if (flagY)
  {
    detachInterrupt(input_pin_y);
    memset(y, 0, sizeof(y));
    for (i = 4; i > 0; i--)
    {
      y[i] = digitalRead(input_pin_y);
      delayNanoseconds(100); /* Need to check with the input */
    }
    flagY = 0;
    flag_ey = 1;
  }

  if (flagX == 0 && flagY == 0 && flag_ex == 1 && flag_ey == 1)
  {
    out_y = y[0] << 3 | y[1] << 2 | y[2] << 1 | y[3] << 0;
    out_x = x[0] << 3 | x[1] << 2 | x[2] << 1 | x[3] << 0;
    if (user_a == out_x)
    {

      if (user_b == out_y)
      {
        digitalWrite(output_pin, LOW);
        delayNanoseconds(108);
        for (i = 0; i < 8; i++)
        {
          digitalWrite(output_pin, bitRead(user_z, i));
          delayNanoseconds(108);
        }
        digitalWrite(output_pin, HIGH);
        digitalWrite(enable_pin, HIGH);
        delayNanoseconds(100);
        digitalWrite(enable_pin, LOW);
      }
    }

    flag_ex = 0;
    flag_ey = 0;
    attachInterrupt(digitalPinToInterrupt(input_pin_x), read_x, FALLING);
    attachInterrupt(digitalPinToInterrupt(input_pin_y), read_y, FALLING);
  }
}