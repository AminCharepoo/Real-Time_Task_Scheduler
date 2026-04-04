#ifndef BUTTONTASK_H
#define BUTTONTASK_H

#include "Task.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include <functional>

class ButtonTask : public Task 
{
private:
    int button_pin;
    std::function<void()> action; // function pointer for the action to perform on button press
    static ButtonTask* instance[30]; // static instance pointer for ISR access (supports 30 GPIO pins)
public:
    ButtonTask(
        std::string name, 
        priority_level priority, 
        interrupt_behavior behavior, 
        int button_pin, 
        std::function<void()> action, 
        Queue& queue
    );
    
    void setup();

    void execute() override;

    static void button_isr(uint gpio, uint32_t events);
};

#endif