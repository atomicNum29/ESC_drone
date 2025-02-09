#include <Arduino.h>

// GPIO pins
#define RECEIVER_CH1 5
#define RECEIVER_CH2 4
#define RECEIVER_CH3 3
#define RECEIVER_CH4 2

#define ESC_FRONT_LEFT 6
#define ESC_FRONT_RIGHT 7
#define ESC_REAR_RIGHT 8
#define ESC_REAR_LEFT 9

// for radio control
#include "Receiver.hpp"
Receiver rc(RECEIVER_CH1, RECEIVER_CH2, RECEIVER_CH3, RECEIVER_CH4);

// for IMU
#include "IMUMadgwickFilter.hpp"
IMUMadgwickFilter filter;

// for motor control
#include "QuadESCManager.hpp"
QuadESCManager qesc(ESC_FRONT_LEFT, ESC_FRONT_RIGHT, ESC_REAR_RIGHT, ESC_REAR_LEFT);

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	rc.begin(); // radio control

	filter.begin(); // IMU

	qesc.begin(); // motor control

	// wait for the throttle signal to be in the range of 1000~1200us for safety
	while (rc.getThrottle() < 1000 || rc.getThrottle() > 1200)
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
	filter.update();

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

	qesc.write(esc1_output, esc2_output, esc3_output, esc4_output);

	char buffer[400];
	sprintf(buffer,
			">CH3: %4d, CH1: %3d, CH2: %3d, CH4: %3d, Roll: %5.1f, Pitch: %5.1f, Yaw: %5.1f, ESC1: %4d, ESC2: %4d, ESC3: %4d, ESC4: %4d\r\n",
			throttle, roll, pitch, yaw, filter.getRoll(), filter.getPitch(), filter.getYaw(), esc1_output, esc2_output, esc3_output, esc4_output);
	Serial.print(buffer);
}