#pragma once // Only include this header file once

#include <Arduino.h>
#include <Servo.h>

class QuadESCManager
{
private:
	Servo esc[4];
	int escPins[4];
	const int ESC_MIN = 1000;
	const int ESC_MAX = 2000;

public:
	QuadESCManager(int esc_front_left = 6, int esc_front_right = 7, int esc_rear_right = 8, int esc_rear_left = 9);
	void begin();
	void write(int FL_pulse_width, int FR_pulse_width, int RR_pulse_width, int RL_pulse_width);
};