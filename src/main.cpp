#include <Arduino.h>
#include <Wire.h>
#include <Arduino_BMI270_BMM150.h>


// 칼만 필터를 위한 변수 선언
float Q_angle = 0.001;	// 각도에 대한 프로세스 노이즈 공분산
float Q_bias = 0.003;	// 바이어스에 대한 프로세스 노이즈 공분산
float R_measure = 0.03; // 측정 노이즈 공분산

float angle = 0.0;				  // 각도 추정값
float bias = 0.0;				  // 바이어스 추정값
float P[2][2] = {{0, 0}, {0, 0}}; // 공분산 행렬

unsigned long lastTime;
float dt;

void setup()
{
	Serial.begin(115200);
	while (!Serial)
		;

	// 센서 초기화
	if (!IMU.begin())
	{
		Serial.println("Failed to initialize IMU!");
		while (1)
			;
	}
	Serial.println("IMU initialized successfully.");

	lastTime = millis();
}

void loop()
{
	float accelX, accelY, accelZ;
	float gyroX, gyroY, gyroZ;

	// 가속도 데이터 읽기
	if (IMU.accelerationAvailable())
	{
		IMU.readAcceleration(accelX, accelY, accelZ);
	}

	// 자이로스코프 데이터 읽기
	if (IMU.gyroscopeAvailable())
	{
		IMU.readGyroscope(gyroX, gyroY, gyroZ);
	}

	// 시간 간격 계산
	unsigned long currentTime = millis();
	dt = (currentTime - lastTime) / 1000.0;
	lastTime = currentTime;

	// 가속도계를 이용한 각도 계산
	float accelAngle = atan2(accelY, accelZ) * RAD_TO_DEG;

	// 칼만 필터 적용
	float rate = gyroX - bias;
	angle += dt * rate;

	// 공분산 행렬 업데이트
	P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
	P[0][1] -= dt * P[1][1];
	P[1][0] -= dt * P[1][1];
	P[1][1] += Q_bias * dt;

	// 측정 업데이트
	float S = P[0][0] + R_measure;
	float K[2];
	K[0] = P[0][0] / S;
	K[1] = P[1][0] / S;

	float y = accelAngle - angle;
	angle += K[0] * y;
	bias += K[1] * y;

	// 공분산 행렬 업데이트
	float P00_temp = P[0][0];
	float P01_temp = P[0][1];

	P[0][0] -= K[0] * P00_temp;
	P[0][1] -= K[0] * P01_temp;
	P[1][0] -= K[1] * P00_temp;
	P[1][1] -= K[1] * P01_temp;

	// 결과 출력
	Serial.print("Angle: ");
	Serial.println(angle);

	delay(10); // 100Hz 주기 유지
}