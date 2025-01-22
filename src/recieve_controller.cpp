#include "recieve_controller.h"

volatile int CH1_pulse_width = 0;
volatile int CH2_pulse_width = 0;
volatile int CH3_pulse_width = 0;
volatile int CH4_pulse_width = 0;

void R9DS_init()
{
	pinMode(CH1, INPUT);
	pinMode(CH2, INPUT);
	pinMode(CH3, INPUT);
	pinMode(CH4, INPUT);
	attachInterrupt(digitalPinToInterrupt(CH1), pulseIn1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(CH2), pulseIn2, CHANGE);
	attachInterrupt(digitalPinToInterrupt(CH3), pulseIn3, CHANGE);
	attachInterrupt(digitalPinToInterrupt(CH4), pulseIn4, CHANGE);
}

void pulseIn1()
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
void pulseIn2()
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
void pulseIn3()
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
void pulseIn4()
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

