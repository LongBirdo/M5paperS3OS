#pragma once
#include <Arduino.h> 

extern const uint16_t screensaver_data[] PROGMEM;
extern const int screensaver_width;
extern const int screensaver_height;

void showScreensaver();