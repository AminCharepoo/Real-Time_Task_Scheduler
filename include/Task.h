#ifndef TASK_H
#define TASK_H

#include "Queue.h"
#include "types.h"
#include <string>

class Task 
{
private:
    std::string name;
    priority_level priority;
    interrupt_behavior behavior;
protected:
    Queue& queue; // reference to the task queue for scheduling so children can access it
public:
    Task(std::string name, priority_level priority, interrupt_behavior behavior, Queue& queue);

    priority_level getPriority() const;
    interrupt_behavior getInterruptBehavior() const;
    std::string getName() const;

    virtual void execute() {}; // virtual function to be implemented by child classes

    virtual ~Task() {};
};

#endif