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
        instance[button_pin] = this; 
        printf("Registered button '%s' on pin %d\n", name.c_str(), button_pin);
    }
    
void ButtonTask::setup() {
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
    gpio_set_irq_enabled_with_callback(button_pin, GPIO_IRQ_EDGE_FALL, true, button_isr); // set up interrupt on falling edge (button press)
}

void ButtonTask::execute() {
    printf("Button execute() called\n");
    if (action) {
        printf("Executing button action\n");
        action(); // execute the assigned action
    } else {
        printf("No action assigned to button task\n");
    }
}

void ButtonTask::button_isr(uint gpio, uint32_t events) { // ISR for button press
    static uint32_t last_press[30] = {0}; // track last press time for debouncing
    uint32_t now = to_ms_since_boot(get_absolute_time()); // get current time in ms
    printf("ISR fired on pin %d\n", gpio);

    if (now - last_press[gpio] < 200) {
        printf("Debounced (too soon) \n");
        return;  // debounce
    }
    last_press[gpio] = now;
    printf("Enqueueing button task\n");
    instance[gpio]->queue.enqueue(instance[gpio]); // enqueue the button task for execution in the main loop
}

ButtonTask* ButtonTask::instance[30] = {nullptr}; 