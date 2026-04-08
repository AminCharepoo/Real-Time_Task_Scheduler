#include "Cases.h"
#include "Queue.h"
#include "TimerTask.h"
#include "ButtonTask.h"
#include "DisplayTask.h"
#include "utils.h"
#include <stdio.h>
#include "pico/stdlib.h"

static TimerTask* timer1 = nullptr;
static ButtonTask* button1 = nullptr;
static DisplayTask* display1 = nullptr;
static ButtonTask* button2 = nullptr;
static TimerTask* timer2 = nullptr;
static TimerTask* timer3 = nullptr;

void cleanupCase()
{
    delete timer1;
    timer1 = nullptr;

    delete button1;
    button1 = nullptr;

    delete display1;
    display1 = nullptr;

    delete button2;
    button2 = nullptr;

    delete timer2;
    timer2 = nullptr;

    delete timer3;
    timer3 = nullptr;
}

void setupCase(CaseMode mode, Queue& queue)
{
    // In case setupCase is ever called more than once,
    // clean old objects first
    cleanupCase();

    switch (mode) {
        case DEFAULT_MODE:
            timer1 = new TimerTask("Timer1", LOW, DROP, 500, toggle_led1, queue);
            button1 = new ButtonTask("Button1", HIGH, FRONT_QUEUE, 11, toggle_led2, queue);
            display1 = new DisplayTask("Display1", HIGH, FRONT_QUEUE, "Hello, World! ", 16, 300, queue);

            button2 = new ButtonTask(
                "Button2",
                VIP,
                FRONT_QUEUE,
                9,
                []() {
                    if (display1) {
                        display1->displayVipMessage("VIP Message!");
                    }
                },
                queue
            );

            timer1->start();
            button1->setup();
            display1->setup();
            button2->setup();
            printf("Running DEFAULT_MODE\n");
            break;

        case EDGE_CASE_1:
            display1 = new DisplayTask(
                "LongDisplay",
                LOW,
                FRONT_QUEUE,
                "THIS MESSAGE IS TOO LONG FOR THE LCD SCREEN",
                3,
                300,
                queue
            );

            button2 = new ButtonTask(
                "Button2",
                VIP,
                FRONT_QUEUE,
                9,
                []() {
                    if (display1) {
                        display1->displayVipMessage("VIP Message!");
                    }
                },
                queue
            );

            display1->setup();
            button2->setup();
            printf("Running EDGE_CASE_1\n");
            break;

        case EDGE_CASE_2:
            timer2 = new TimerTask("Timer2", LOW, DROP, 300, toggle_led1, queue);
            timer3 = new TimerTask("Timer3", LOW, DROP, 200, toggle_led2, queue);

            timer2->start();
            timer3->start();
            printf("Running EDGE_CASE_2\n");
            break;

        case EDGE_CASE_3:
            display1 = new DisplayTask(
                "EmptyDisplay",
                LOW,
                FRONT_QUEUE,
                "Queue Empty   ",
                16,
                500,
                queue
            );

            display1->setup();
            printf("Running EDGE_CASE_3\n");
            break;
    }
}