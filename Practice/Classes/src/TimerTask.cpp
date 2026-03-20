#include "TimerTask.h"

TimerTask::TimerTask(std::string name, 
    priority_level priority, 
    interrupt_behavior interrupt_behavior, 
    uint32_t interval_ms, 
    void (*action)(), 
    Queue& queue
    ) :
    Task(name, priority, interrupt_behavior, queue), 
    interval_ms(interval_ms), 
    action(action) 
    {}

void TimerTask::start() {
    add_repeating_timer_ms(interval_ms, timer_callback, this, &timer); // start the repeating timer with the specified interval and callback
}

void TimerTask::stop() {
    cancel_repeating_timer(&timer); // stop the repeating timer
}

void TimerTask::execute() {
    if (action) action(); // execute the assigned action when the timer triggers
}

bool TimerTask::timer_callback(struct repeating_timer *t) { // callback funciton for the timer
    TimerTask* task = static_cast<TimerTask*>(t->user_data); // get the TimerTask instance from the timer's user data
    task->queue.enqueue(task); // enqueue the timer task for execution in the main loop
    return true; 
}