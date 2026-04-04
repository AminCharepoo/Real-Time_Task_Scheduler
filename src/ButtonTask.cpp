#include "ButtonTask.h"

ButtonTask::ButtonTask(
    std::string name, 
    priority_level priority, 
    interrupt_behavior behavior, 
    int button_pin, 
    std::function<void()> action,
    Queue& queue
    ) : 
    Task(name, priority, behavior, queue), 
    button_pin(button_pin), 
    action(action) 
    {
        instance = this; 
    }
    
void ButtonTask::setup() {
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
    gpio_set_irq_enabled_with_callback(button_pin, GPIO_IRQ_EDGE_FALL, true, button_isr); // set up interrupt on falling edge (button press)
}

void ButtonTask::execute() {
    if (action) action(); // execute the assigned action
}

void ButtonTask::button_isr(uint gpio, uint32_t events) { // ISR for button press
    static uint32_t last_press = 0; // track last press time for debouncing
    uint32_t now = to_ms_since_boot(get_absolute_time()); // get current time in ms
    if (now - last_press < 200) return;  // debounce
    last_press = now;
    instance->queue.enqueue(instance); // enqueue the button task for execution in the main loop
}

ButtonTask* ButtonTask::instance = nullptr; 