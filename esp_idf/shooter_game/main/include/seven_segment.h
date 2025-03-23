// main/include/seven_segment.h
#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "driver/gpio.h"

// Seven segment pins
#define SEG_A_PIN 19
#define SEG_B_PIN 18
#define SEG_C_PIN 26
#define SEG_D_PIN 25
#define SEG_E_PIN 33
#define SEG_F_PIN 23
#define SEG_G_PIN 27

// Initialize seven segment display
void seven_segment_init(void);

// Display a digit (0-9)
void seven_segment_display(int digit);

#endif /* SEVEN_SEGMENT_H */