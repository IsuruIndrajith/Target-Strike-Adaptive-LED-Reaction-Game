// main/include/display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"

// Display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Initialize the OLED display
void display_init(void);

// Update display with text
void update_display(const char* line1, const char* line2);

// Display centered text
void display_centered_text(const char* text, int y);

// Clear the display
void display_clear(void);

#endif /* DISPLAY_H */