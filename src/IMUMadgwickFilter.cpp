#include "IMUMadgwickFilter.hpp"

IMUMadgwickFilter::IMUMadgwickFilter()
{
}

void IMUMadgwickFilter::begin()
{
	IMU.begin();
	filter.begin(IMU.gyroscopeSampleRate());
	microsPerReading = 1000000 / IMU.gyroscopeSampleRate();
	microsPrevious = micros();
}

void IMUMadgwickFilter::update()
{
	float ax, ay, az;
	float gx, gy, gz;
	float AX, AY, AZ;
	float GX, GY, GZ;
	unsigned long microsNow;

	microsNow = micros();
	if (microsNow - microsPrevious >= microsPerReading)
	{
		if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
		{
			IMU.readAcceleration(ax, ay, az);
			IMU.readGyroscope(gx, gy, gz);
			AX = ay;
			AY = -ax;
			AZ = az;
			GX = -gy;
			GY = gx;
			GZ = -gz;

			filter.updateIMU(GX, GY, GZ, AX, AY, AZ);

			microsPrevious = microsPrevious + microsPerReading;
		}
	}
}

float IMUMadgwickFilter::getRoll()
{
	return filter.getRoll();
}

float IMUMadgwickFilter::getPitch()
{
	return filter.getPitch();
}

float IMUMadgwickFilter::getYaw()
{
	return filter.getYaw();
}
