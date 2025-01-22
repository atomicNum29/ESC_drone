#include <Arduino.h>

#define CH1 5
#define CH2 4
#define CH3 3
#define CH4 2

extern volatile int CH1_pulse_width;
extern volatile int CH2_pulse_width;
extern volatile int CH3_pulse_width;
extern volatile int CH4_pulse_width;

void pulseIn1();
void pulseIn2();
void pulseIn3();
void pulseIn4();

void R9DS_init();
