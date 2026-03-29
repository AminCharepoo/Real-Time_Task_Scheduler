#ifndef QUEUE_H
#define QUEUE_H

class Task; // forward declaration to avoid circular dependency

#define QUEUE_SIZE 10
class Queue {
private:
    Task* tasks[QUEUE_SIZE]; // Array of tasks
    volatile int head; // front task
    volatile int tail; // last task
    volatile bool vip_pending; // flag to indicate if a VIP task is pending
public:
    Queue();

    bool full(); // check if queue is full
    bool empty(); // check if queue is empty

    void push_back(Task* task);
    void push_front(Task* task);

    void enqueue(Task* task);
    Task* dequeue();

    bool isVipPending() const; // check if a VIP task is pending
    void clearVipPending(); // clear the VIP pending flag
};

#endif