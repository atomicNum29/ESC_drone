#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include "recieve_controller.h"

#define THROTTLE CH3_pulse_width
#define YAW CH4_pulse_width
#define PITCH CH2_pulse_width
#define ROLL CH1_pulse_width

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

void setup()
{
	Serial.begin(115200);

	R9DS_init();

	for (int i = 0; i < numEscs; i++)
	{
		esc[i].attach(escPins[i], ESC_MIN, ESC_MAX);
	}
}

void loop()
{
	int throttle = map(THROTTLE, 1000, 2000, 1000, 2000);
	int roll = map(ROLL, 1000, 2000, -500, 500); // 조정 가능한 범위
	int pitch = map(PITCH, 1000, 2000, -500, 500);
	int yaw = map(YAW, 1000, 2000, -500, 500);

	int esc1_output = throttle + pitch + roll - yaw;
	int esc2_output = throttle + pitch - roll + yaw;
	int esc3_output = throttle - pitch - roll - yaw;
	int esc4_output = throttle - pitch + roll + yaw;

	// PWM 값 제한 (1000~2000us)
	esc1_output = constrain(esc1_output, 1000, 2000);
	esc2_output = constrain(esc2_output, 1000, 2000);
	esc3_output = constrain(esc3_output, 1000, 2000);
	esc4_output = constrain(esc4_output, 1000, 2000);

	esc[0].writeMicroseconds(esc1_output);
	esc[1].writeMicroseconds(esc2_output);
	esc[2].writeMicroseconds(esc3_output);
	esc[3].writeMicroseconds(esc4_output);

	Serial.print("ESC1: ");
	Serial.print(esc1_output);
	Serial.print("\tESC2: ");
	Serial.print(esc2_output);
	Serial.print("\tESC3: ");
	Serial.print(esc3_output);
	Serial.print("\tESC4: ");
	Serial.print(esc4_output);
	Serial.println();
}