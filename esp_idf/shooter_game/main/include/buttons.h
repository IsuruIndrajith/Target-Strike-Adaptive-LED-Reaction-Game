// main/include/buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include "driver/gpio.h"

// Button pins
#define BUTTON1_PIN 12
#define BUTTON2_PIN 13
#define BUTTON3_PIN 5

// Initialize buttons
void buttons_init(void);

// Read button states
int read_button1(void);
int read_button2(void);
int read_button3(void);

#endif /* BUTTONS_H */