#ifndef UTILS_H
#define UTILS_H

#include "hardware/gpio.h"
#include <stdio.h>

void toggle_led(int pin);

#define LED1_PIN 15
void toggle_led1();

#define LED2_PIN 13
void toggle_led2();

#endif