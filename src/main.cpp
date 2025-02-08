#include <Arduino.h>
#include <Wire.h>

// for motor control
#include <Servo.h>

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

// for radio control
#include "Receiver.hpp"
Receiver rc;

// for IMU
#include <Arduino_BMI270_BMM150.h>
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	IMU.begin();
	filter.begin(IMU.gyroscopeSampleRate());
	microsPerReading = 1000000 / IMU.gyroscopeSampleRate();
	microsPrevious = micros();

	rc.begin();

	for (int i = 0; i < numEscs; i++)
	{
		esc[i].attach(escPins[i], ESC_MIN, ESC_MAX);
		esc[i].writeMicroseconds(0);
	}

	while (rc.getThrottle() < 1000)
	{
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
	}
	digitalWrite(LED_BUILTIN, LOW);
}

void loop()
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

			// roll = filter.getRoll();
			// pitch = filter.getPitch();
			// heading = filter.getYaw();
			// Serial.print("Roll: ");
			// Serial.print(roll);
			// Serial.print("\tPitch: ");
			// Serial.print(pitch);
			// Serial.print("\tHeading: ");
			// Serial.println(heading);

			microsPrevious = microsPrevious + microsPerReading;
		}
	}

	// === 목표 자세 (조종기 입력) ===
	int throttle = map(rc.getThrottle(), 1000, 2000, 1000, 2000);
	int roll = map(rc.getRoll(), 1000, 2000, -50, 50); // 조정 가능한 범위
	int pitch = map(rc.getPitch(), 1000, 2000, -50, 50);
	int yaw = map(rc.getYaw(), 1000, 2000, -50, 50);

	float targetPitch = pitch; // 조종기로 설정
	float targetRoll = roll;
	float targetYaw = yaw;

	float pitchError = targetPitch - filter.getPitch();
	float rollError = targetRoll - filter.getRoll();

	// === 비례 게인 (조정 필요) ===
	float Kp_pitch = 20;
	float Kp_roll = 20;
	float Kp_yaw = 50;

	// === 제어 신호 ===
	float controlPitch = Kp_pitch * pitchError;
	float controlRoll = Kp_roll * rollError;
	float controlYaw = Kp_yaw * targetYaw;

	int esc1_output = throttle + controlPitch + controlRoll - controlYaw;
	int esc2_output = throttle + controlPitch - controlRoll + controlYaw;
	int esc3_output = throttle - controlPitch - controlRoll - controlYaw;
	int esc4_output = throttle - controlPitch + controlRoll + controlYaw;

	// PWM 값 제한 (1000~2000us)
	esc1_output = constrain(esc1_output, 1000, 2000);
	esc2_output = constrain(esc2_output, 1000, 2000);
	esc3_output = constrain(esc3_output, 1000, 2000);
	esc4_output = constrain(esc4_output, 1000, 2000);

	esc[0].writeMicroseconds(esc1_output);
	esc[1].writeMicroseconds(esc2_output);
	esc[2].writeMicroseconds(esc3_output);
	esc[3].writeMicroseconds(esc4_output);

	Serial.print(">Throttle:");
	Serial.print(throttle);
	Serial.print(",\tRoll:");
	Serial.print(roll);
	Serial.print(",\tPitch:");
	Serial.print(pitch);
	Serial.print(",\tYaw:");
	Serial.print(yaw);
	Serial.print(",\tESC1:");
	Serial.print(esc1_output);
	Serial.print(",\tESC2:");
	Serial.print(esc2_output);
	Serial.print(",\tESC3:");
	Serial.print(esc3_output);
	Serial.print(",\tESC4:");
	Serial.print(esc4_output);
	Serial.print("\r\n");
}