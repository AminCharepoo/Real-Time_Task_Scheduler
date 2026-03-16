#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

using namespace std;


enum priority_level {
    LOW = 1,
    HIGH = 2,
    VIP = 3
};
enum interrupt_behavior {
    DROP = 1,
    FRONT_QUEUE = 2,
};

volatile bool vip_pending = false; // flag to indicate if a VIP task is pending

class Queue;

class Task 
{
private:
    string name;
    priority_level priority;
    interrupt_behavior behavior;
protected:
    Queue& queue; // reference to the task queue for scheduling so children can access it
public:
    Task(string name, priority_level priority, interrupt_behavior behavior, Queue& queue) : 
        name(name), 
        priority(priority), 
        behavior(behavior),
        queue(queue) {}

    priority_level getPriority() const { return priority; }
    interrupt_behavior getInterruptBehavior() const { return behavior; }
    string getName() const { return name; }

    virtual void execute() {} // virtual function to be implemented by child classes

    virtual ~Task() {}
};

#define QUEUE_SIZE 10
class Queue {
private:
    Task* tasks[QUEUE_SIZE]; // Array of tasks
    volatile int head; // front task
    volatile int tail; // last task
public:
    Queue() : head(0), tail(0) {}

    bool full() { return (tail + 1) % QUEUE_SIZE == head; } // check if queue is full
    bool empty() { return head == tail; } // check if queue is empty

    void push_back(Task* task) {
        if (full()) return;
        tasks[tail] = task; // insert task at tail
        tail = (tail + 1) % QUEUE_SIZE; // move tail forward
    }

    void push_front(Task* task) {
        if (full()) return;
        head = (head - 1 + QUEUE_SIZE) % QUEUE_SIZE; // move head backward
        tasks[head] = task; // inert task at new head
    }

    void enqueue(Task* task) { // have to implement enqueue here since it needs to access the Task's priority and behavior
        if (full()) return;
        if (task->getPriority() == VIP) { vip_pending = true; } // if the new task is VIP, set the vip_pending flag
        push_back(task);
    }


    Task* dequeue() {
        if (empty()) return nullptr;

        int highest_priority_index = head; 
        for (int i = (head + 1) % QUEUE_SIZE; i != tail; i = (i+1) % QUEUE_SIZE) {
            if (tasks[i]->getPriority() > tasks[highest_priority_index]->getPriority()) { highest_priority_index = i; }
        }

        Task* task = tasks[highest_priority_index]; // get task at head
        tasks[highest_priority_index] = tasks[head];
        head = (head + 1) % QUEUE_SIZE; // move head forward
        return task;    
    }
};




class ButtonTask : public Task 
{
private:
    int button_pin;
    void (*action) (); // function pointer for the action to perform on button press
    static ButtonTask* instance; // static instance pointer for ISR access
public:
    ButtonTask(string name, priority_level priority, interrupt_behavior interrupt_behavior, int button_pin, void (*action)(), Queue& queue) : 
        Task(name, priority, interrupt_behavior, queue), 
        button_pin(button_pin), 
        action(action) {
            instance = this;
        }
    
    void setup() {
        gpio_init(button_pin);
        gpio_set_dir(button_pin, GPIO_IN);
        gpio_pull_up(button_pin);
        gpio_set_irq_enabled_with_callback(button_pin, GPIO_IRQ_EDGE_FALL, true, button_isr); // set up interrupt on falling edge (button press)
    }

    void execute() override {
        if (action) action(); // execute the assigned action
    }

    static void button_isr(uint gpio, uint32_t events) { // ISR for button press
        static uint32_t last_press = 0; // track last press time for debouncing
        uint32_t now = to_ms_since_boot(get_absolute_time()); // get current time in ms
        if (now - last_press < 200) return;  // debounce
        last_press = now;
        instance->queue.enqueue(instance); // enqueue the button task for execution in the main loop
    }
};
ButtonTask* ButtonTask::instance = nullptr; 

class TimerTask: public Task 
{
private:
    uint32_t interval_ms;
    struct repeating_timer timer; // timer structure for repeating timer
    void (*action) (); // function pointer for the action to perform on timer trigger
public:
    TimerTask(string name, priority_level priority, interrupt_behavior interrupt_behavior, uint32_t interval_ms, void (*action)(), Queue& queue) : 
        Task(name, priority, interrupt_behavior, queue), 
        interval_ms(interval_ms), 
        action(action) {}

    void start() {
        add_repeating_timer_ms(interval_ms, timer_callback, this, &timer); // start the repeating timer with the specified interval and callback
    }

    void stop() {
        cancel_repeating_timer(&timer); // stop the repeating timer
    }

    void execute() override {
        if (action) action(); // execute the assigned action when the timer triggers
    }

    static bool timer_callback(struct repeating_timer *t) { // callback funciton for the timer
        TimerTask* task = static_cast<TimerTask*>(t->user_data); // get the TimerTask instance from the timer's user data
        task->queue.enqueue(task); // enqueue the timer task for execution in the main loop
        return true; 
    }
};

void toggle_led(int pin) {
    gpio_xor_mask(1u << pin); // toggle the specified LED pin
    printf("Led %i toggled\n", pin);
}

#define LED1_PIN 15
void toggle_led1() {
    toggle_led(LED1_PIN); 
}

#define LED2_PIN 14
void toggle_led2() {
    toggle_led(LED2_PIN);
}



int main()
{
    stdio_init_all();
    sleep_ms(3000); // wait for 2 seconds to allow time for the USB connection to be established

    printf("System Ready\n");

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);


    Queue queue;

    TimerTask timer1("Timer1", LOW, DROP, 500, toggle_led1, queue);
    ButtonTask button("Button1", HIGH, FRONT_QUEUE, 16, toggle_led2, queue);

    timer1.start();
    button.setup();

    printf("Setup complete\n");

    Task* interrupted_task = nullptr;

    while (true) {
        Task* t = queue.dequeue();

        if (t){
            t->execute();
            if (vip_pending && t->getInterruptBehavior() == FRONT_QUEUE) { interrupted_task = t; } // if there is a VIP task waiting and the current task should be requeued at front store it in interrupted task

            if (t->getPriority() == VIP) {
                vip_pending = false;
                if (interrupted_task) { // if there is an interrupted task requeue it in the front
                    queue.push_front(interrupted_task);
                    interrupted_task = nullptr;
                }
            }
        }
        else tight_loop_contents(); // if no tasks to execute, run tight loop to save power
    }

    return 0;
}
