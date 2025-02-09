#include "QuadESCManager.hpp"

QuadESCManager::QuadESCManager(int esc_front_left, int esc_front_right, int esc_rear_right, int esc_rear_left)
{
	escPins[0] = esc_front_left;
	escPins[1] = esc_front_right;
	escPins[2] = esc_rear_right;
	escPins[3] = esc_rear_left;
}

void QuadESCManager::begin()
{
	for (int i = 0; i < 4; i++)
	{
		esc[i].attach(escPins[i], ESC_MIN, ESC_MAX);
		esc[i].writeMicroseconds(0);
	}
}

void QuadESCManager::write(int FL_pulse_width, int FR_pulse_width, int RR_pulse_width, int RL_pulse_width)
{
	esc[0].writeMicroseconds(FL_pulse_width);
	esc[1].writeMicroseconds(FR_pulse_width);
	esc[2].writeMicroseconds(RR_pulse_width);
	esc[3].writeMicroseconds(RL_pulse_width);
}
