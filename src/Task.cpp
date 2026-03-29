#include "Task.h"
#include "Queue.h"

Task::Task(std::string name, priority_level priority, interrupt_behavior behavior, Queue& queue) : 
        name(name), 
        priority(priority), 
        behavior(behavior),
        queue(queue) {}

priority_level Task::getPriority() const { return priority; }
interrupt_behavior Task::getInterruptBehavior() const { return behavior; }
std::string Task::getName() const { return name; }


