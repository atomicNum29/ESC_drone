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

// for PID control
#include <PID_v1.h>
double rollSetpoint, rollInput, rollOutput;
PID rollPID(&rollInput, &rollOutput, &rollSetpoint, 2, 5, 1, DIRECT);
double pitchSetpoint, pitchInput, pitchOutput;
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, 2, 5, 1, DIRECT);

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

	rollPID.SetMode(AUTOMATIC);
	rollPID.SetOutputLimits(-50, 50);
	rollPID.SetSampleTime(10); // 10ms
	pitchPID.SetMode(AUTOMATIC);
	pitchPID.SetOutputLimits(-50, 50);
	pitchPID.SetSampleTime(10); // 10ms

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

	rollSetpoint = roll;
	pitchSetpoint = pitch;
	rollInput = filter.getRoll();
	pitchInput = filter.getPitch();

	rollPID.Compute();
	pitchPID.Compute();	

	int esc1_output = throttle + pitchOutput + rollOutput - yaw;
	int esc2_output = throttle + pitchOutput - rollOutput + yaw;
	int esc3_output = throttle - pitchOutput - rollOutput - yaw;
	int esc4_output = throttle - pitchOutput + rollOutput + yaw;

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