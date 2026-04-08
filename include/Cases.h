#ifndef CASES_H
#define CASES_H

#include "utils.h"
#include "Queue.h"

// Enum for selecting which case to run
enum CaseMode {
    DEFAULT_MODE, // This is our normal original main with the sample of a Real-Time Task Scheduler in action
    EDGE_CASE_1,  // Tests long message that doesn't fit on LCD screen, should display, "MSG TOO LONG!"
    EDGE_CASE_2,  // Tests two timers running at different intervals to check to stress the queue and ensure proper scheduling, LEDs should flicker
    EDGE_CASE_3   // Tests empty queue scencario, should display "Queue Empty" on LLCD screen
};

// Function to set up the selected case
void setupCase(CaseMode mode, Queue& queue);
// Function to clean up any dynamically allocated objects
void cleanupCase();

#endif