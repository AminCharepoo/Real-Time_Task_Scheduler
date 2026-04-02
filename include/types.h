#ifndef TYPES_H
#define TYPES_H

enum priority_level {
    LOW = 1,
    HIGH = 2,
    VIP = 3
};
enum interrupt_behavior {
    DROP = 1,
    FRONT_QUEUE = 2,
};

#endif