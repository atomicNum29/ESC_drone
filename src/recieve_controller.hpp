#ifndef RECIEVE_CONTROLLER_HPP
#define RECIEVE_CONTROLLER_HPP

#include <Arduino.h>

class RecieveController
{
private:
	static int CH1;
	static int CH2;
	static int CH3;
	static int CH4;
	static volatile int CH1_pulse_width;
	static volatile int CH2_pulse_width;
	static volatile int CH3_pulse_width;
	static volatile int CH4_pulse_width;
	static void pulseIn1();
	static void pulseIn2();
	static void pulseIn3();
	static void pulseIn4();

public:
	RecieveController(int ch1 = 5, int ch2 = 4, int ch3 = 3, int ch4 = 2);
	void begin();
	int getThrottle();
	int getRoll();
	int getPitch();
	int getYaw();
};

#endif // RECIEVE_CONTROLLER_HPP