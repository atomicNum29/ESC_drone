#pragma once // Only include this header file once

#include <Arduino.h>
#include <Arduino_BMI270_BMM150.h>
#include <MadgwickAHRS.h>

class IMUMadgwickFilter
{
private:
	Madgwick filter;
	unsigned long microsPerReading, microsPrevious;

public:
	IMUMadgwickFilter();
	void begin();
	void update();
	float getRoll();
	float getPitch();
	float getYaw();
};