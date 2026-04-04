#include <stdio.h>
#include "pico/stdlib.h"
#include "TimerTask.h"
#include "ButtonTask.h"
#include "DisplayTask.h"
#include "utils.h"

int main()
{
    stdio_init_all();
    sleep_ms(3000); // wait for 2 seconds to allow time for the USB connection to be established

    printf("System Ready\n");

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);

    Queue queue;

    TimerTask timer1("Timer1", LOW, DROP, 500, toggle_led1, queue);
    ButtonTask button("Button1", HIGH, FRONT_QUEUE, 11, toggle_led2, queue);
    DisplayTask display("Display1", LOW, FRONT_QUEUE, "Hello, World! ", 3, 300, queue);
    ButtonTask button2("Button2", VIP, FRONT_QUEUE, 9, [&display]() {display.displayVipMessage("VIP Message!"); }, queue);

    timer1.start();
    button.setup();
    display.setup();
    button2.setup();

    printf("Setup complete\n");

    Task* interrupted_task = nullptr;

    while (true) {
        Task* t = queue.dequeue();

        if (t){
            t->execute();
            if (queue.isVipPending() && t->getInterruptBehavior() == FRONT_QUEUE) { interrupted_task = t; } // if there is a VIP task waiting and the current task should be requeued at front store it in interrupted task

            if (t->getPriority() == VIP) {
                queue.clearVipPending(); // clear VIP flag if we just executed a VIP task
                if (interrupted_task) { // if there is an interrupted task requeue it in the front
                    queue.push_front(interrupted_task);
                    interrupted_task = nullptr;
                }
            }
        }
        else tight_loop_contents(); // if no tasks to execute, run tight loop to save power
    }

    return 0;
}
