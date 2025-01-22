#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#define CH1 5
#define CH2 4
#define CH3 3
#define CH4 2

volatile int CH1_pulse_width = 0;
volatile int CH2_pulse_width = 0;
volatile int CH3_pulse_width = 0;
volatile int CH4_pulse_width = 0;

void pulsecount1()
{
	static unsigned long last_time = 0;
	unsigned long current_time = micros();
	if (digitalRead(CH1) == HIGH)
	{
		last_time = current_time;
	}
	else
	{
		CH1_pulse_width = current_time - last_time;
	}
}
void pulsecount2()
{
	static unsigned long last_time = 0;
	unsigned long current_time = micros();
	if (digitalRead(CH2) == HIGH)
	{
		last_time = current_time;
	}
	else
	{
		CH2_pulse_width = current_time - last_time;
	}
}
void pulsecount3()
{
	static unsigned long last_time = 0;
	unsigned long current_time = micros();
	if (digitalRead(CH3) == HIGH)
	{
		last_time = current_time;
	}
	else
	{
		CH3_pulse_width = current_time - last_time;
	}
}
void pulsecount4()
{
	static unsigned long last_time = 0;
	unsigned long current_time = micros();
	if (digitalRead(CH4) == HIGH)
	{
		last_time = current_time;
	}
	else
	{
		CH4_pulse_width = current_time - last_time;
	}
}

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

	pinMode(CH1, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(CH1), pulsecount1, CHANGE);
	pinMode(CH2, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(CH2), pulsecount2, CHANGE);
	pinMode(CH3, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(CH3), pulsecount3, CHANGE);
	pinMode(CH4, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(CH4), pulsecount4, CHANGE);

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