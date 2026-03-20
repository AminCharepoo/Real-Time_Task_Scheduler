#include "utils.h"

void toggle_led(int pin) {
    gpio_xor_mask(1u << pin); // toggle the specified LED pin
    printf("Led %i toggled\n", pin);
}

void toggle_led1() { toggle_led(LED1_PIN); }

void toggle_led2() { toggle_led(LED2_PIN); }