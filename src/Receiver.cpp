#include "Receiver.hpp"

int Receiver::CH1 = 0;
int Receiver::CH2 = 0;
int Receiver::CH3 = 0;
int Receiver::CH4 = 0;
volatile int Receiver::CH1_pulse_width = 0;
volatile int Receiver::CH2_pulse_width = 0;
volatile int Receiver::CH3_pulse_width = 0;
volatile int Receiver::CH4_pulse_width = 0;

void Receiver::pulseIn1()
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
void Receiver::pulseIn2()
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
void Receiver::pulseIn3()
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
void Receiver::pulseIn4()
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

Receiver::Receiver(int ch1, int ch2, int ch3, int ch4)
{
	CH1 = ch1;
	CH2 = ch2;
	CH3 = ch3;
	CH4 = ch4;
	CH1_pulse_width = 0;
	CH2_pulse_width = 0;
	CH3_pulse_width = 0;
	CH4_pulse_width = 0;
}

void Receiver::begin()
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

int Receiver::getThrottle()
{
	return CH3_pulse_width;
}

int Receiver::getRoll()
{
	return CH1_pulse_width;
}

int Receiver::getPitch()
{
	return CH2_pulse_width;
}

int Receiver::getYaw()
{
	return CH4_pulse_width;
}
