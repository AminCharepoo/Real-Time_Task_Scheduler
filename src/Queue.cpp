#include "Queue.h"
#include "Task.h"
#include "types.h"

Queue::Queue() : head(0), tail(0), vip_pending(false) {}

bool Queue::full() { return (tail + 1) % QUEUE_SIZE == head; } // check if queue is full
bool Queue::empty() { return head == tail; } // check if queue is empty

void Queue::push_back(Task* task) {
    if (full()) return;
    tasks[tail] = task; // insert task at tail
    tail = (tail + 1) % QUEUE_SIZE; // move tail forward
}

void Queue::push_front(Task* task) {
    if (full()) return;
    head = (head - 1 + QUEUE_SIZE) % QUEUE_SIZE; // move head backward
    tasks[head] = task; // inert task at new head
}

void Queue::enqueue(Task* task) { // have to implement enqueue here since it needs to access the Task's priority and behavior
    if (full()) return;
    if (task->getPriority() == VIP) { vip_pending = true; } // if the new task is VIP, set the vip_pending flag
    push_back(task);
}


Task* Queue::dequeue() {
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

bool Queue::isVipPending() const { return vip_pending; } // check if a VIP task is pending
void Queue::clearVipPending() { vip_pending = false; } // clear the VIP pending flag
