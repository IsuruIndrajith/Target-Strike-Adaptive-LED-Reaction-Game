// main/include/buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include "driver/gpio.h"

// Buzzer pin
#define BUZZER_PIN 15

// Initialize buzzer
void buzzer_init(void);

// Play a short beep
void buzzer_beep(int duration_ms);

#endif /* BUZZER_H */