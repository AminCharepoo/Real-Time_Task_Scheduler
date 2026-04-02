#include "pico/stdlib.h"   // always include this

int main() {
    // --- Setup (runs once) ---
    gpio_init(25);                     // initialize GPIO pin 25 (onboard LED)
    gpio_set_dir(25, GPIO_OUT);        // set it as output

    // --- Loop (runs forever) ---
    while (true) {
        gpio_put(25, 1);               // LED on
        sleep_ms(1000);                 // wait 1000ms
        gpio_put(25, 0);               // LED off
        sleep_ms(1000);
    }
}