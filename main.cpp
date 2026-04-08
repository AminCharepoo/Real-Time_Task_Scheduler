#include <stdio.h>
#include "pico/stdlib.h"
#include "Task.h"
#include "Cases.h"
#include "utils.h"

// CONFIG (DEFAULT_MODE, EDGE_CASE_1, EDGE_CASE_2, or EDGE_CASE_3)
const CaseMode CURRENT_MODE = DEFAULT_MODE;

int main()
{
    stdio_init_all();
    sleep_ms(3000);

    printf("System Ready\n");

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);

    Queue queue;
    setupCase(CURRENT_MODE, queue);

    printf("Setup complete\n");

    Task* interrupted_task = nullptr;

    while (true) {
        Task* t = queue.dequeue();

        if (t == nullptr) {
            printf("Queue empty - idle state\n");
            sleep_ms(100);
            tight_loop_contents();
            continue;
        }

        t->execute();

        if (queue.isVipPending() && t->getInterruptBehavior() == FRONT_QUEUE) {
            interrupted_task = t;
        }

        if (t->getPriority() == VIP) {
            queue.clearVipPending();
            if (interrupted_task) {
                queue.push_front(interrupted_task);
                interrupted_task = nullptr;
            }
        }
    }

    cleanupCase();
    return 0;
}