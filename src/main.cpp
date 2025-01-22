#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include "recieve_controller.h"

#define ESC_FRONT_LEFT 6
#define ESC_FRONT_RIGHT 7
#define ESC_BACK_RIGHT 8
#define ESC_BACK_LEFT 9

#define ESC_MIN 1000
#define ESC_MAX 2000

Servo esc[4];
// Pins connected to ESCs
const int escPins[] = {ESC_FRONT_LEFT, ESC_FRONT_RIGHT, ESC_BACK_RIGHT, ESC_BACK_LEFT};
const int numEscs = 4;

void setup()
{
	Serial.begin(115200);

	R9DS_init();

	for (int i = 0; i < numEscs; i++)
	{
		esc[i].attach(escPins[i], ESC_MIN, ESC_MAX);
	}
}

void loop()
{
	Serial.print("CH1: ");
	Serial.print(CH1_pulse_width);
	Serial.print("\tCH2: ");
	Serial.print(CH2_pulse_width);
	Serial.print("\tCH3: ");
	Serial.print(CH3_pulse_width);
	Serial.print("\tCH4: ");
	Serial.print(CH4_pulse_width);
	Serial.println();
}