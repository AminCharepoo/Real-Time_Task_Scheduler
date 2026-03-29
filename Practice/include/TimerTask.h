#ifndef TIMERTASK_H
#define TIMERTASK_H

#include "Task.h"
#include "pico/time.h"

class TimerTask: public Task 
{
private:
    uint32_t interval_ms;
    struct repeating_timer timer; // timer structure for repeating timer
    void (*action) (); // function pointer for the action to perform on timer trigger
public:
    TimerTask(
        std::string name, 
        priority_level priority, 
        interrupt_behavior interrupt_behavior, 
        uint32_t interval_ms, 
        void (*action)(), 
        Queue& queue
    );

    void start();

    void stop();

    void execute() override;

    static bool timer_callback(struct repeating_timer *t);
};


#endif