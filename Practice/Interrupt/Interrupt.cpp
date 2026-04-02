#include <stdio.h>
#include "pico/stdlib.h"
// #include "hardware/irq.h"
#include "hardware/timer.h"

#define LED1 15
#define LED2 14
#define LED3 13
#define BUTTON 16

// setup flags for the main loop to check when an interrupt has occurred
volatile bool button_flag = false;
volatile bool timer1_flag = false;
volatile bool timer3_flag = false;

struct repeating_timer timer_led1;
struct repeating_timer timer_led3;

// this function will be called by the timer interrupt
int ticks_led1 = 0;
bool timer_callback_led1(struct repeating_timer *t) {
    ticks_led1++;
    timer1_flag = true;  
    return true; 
}
int ticks_led3 = 0; 
bool timer_callback_led3(struct repeating_timer *t) {
    ticks_led3++;
    timer3_flag = true;
    return true; 
}

// this function will be called by the button interrupt
void button_callback(uint gpio, uint32_t events) {
    static uint32_t last_press = 0; // track the last time the button was pressed
    uint32_t now = to_ms_since_boot(get_absolute_time()); 
    if (now - last_press > 200) { // doebounce the button by ignoring presses that occur within 200ms of the last press
        button_flag = true;
        last_press = now;
    }
}

int main()
{
    stdio_init_all(); // initialize the UART for printf

    // initialize the GPIO pins for the LEDs and button
    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_init(LED2);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_init(LED3);
    gpio_set_dir(LED3, GPIO_OUT);

    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    // set up the button interrupt to call the button_callback function when the button is pressed
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, button_callback); // watch pin BUTTON and whenever it falls from HIGH to LOW run button_callback function
    // set up the timer whinterrupt to call the timer_callback function every 500ms
    add_repeating_timer_ms(500, timer_callback_led1, NULL, &timer_led1); // call timer_callback_led1 every 500ms and store the timer in timer_led1
    add_repeating_timer_ms(1000, timer_callback_led3, NULL, &timer_led3); // call timer_callback_led3 every 1000ms and store the timer in timer_led3

    while (true) {
        if (timer1_flag) { // if the timer interrupt has occurred, toggle LED1 and print the number of ticks
            timer1_flag = false;
            gpio_xor_mask(1u << LED1); // 1u << led1 creates a bitmask with a 1 in the position corresponding to LED1, and gpio_xor_mask toggles the state of that pin
        }

        if (timer3_flag) { // if the timer interrupt has occurred, toggle LED3 and print the number of ticks
            timer3_flag = false;
            gpio_xor_mask(1u << LED3); // 1u << led3 creates a bitmask with a 1 in the position corresponding to LED3, and gpio_xor_mask toggles the state of that pin
        }

        if (button_flag) {
            button_flag = false;
            gpio_xor_mask(1u << LED2); // 1u << led2 creates a bitmask with a 1 in the position corresponding to LED2, and gpio_xor_mask toggles the state of that pin
            printf("Button pressed\n");
            printf("Ticks1: %i\n", ticks_led1);
            printf("Ticks3: %i\n", ticks_led3);
        }
    }
}
