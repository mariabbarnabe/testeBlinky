#include <assert.h>
#include <string.h>

#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "sysinit/sysinit.h"

/* Define task stack and task object */
#define LED_TASK_PRIO        (201)  /* 1 = highest, 255 = lowest */
#define LED_TASK2_PRIO	     (200)
#define LED_STACK_SIZE       OS_STACK_ALIGN(64)

struct os_task led_task;
struct os_task led_task2;
os_stack_t led_task_stack[LED_STACK_SIZE];
os_stack_t led_task2_stack[LED_STACK_SIZE];

static void led_task_func(void *arg);
static void led_task2_func(void *arg);
int
main(int argc, char **argv)
{
    int rc;

    /* Initialize the task */
    os_task_init(&led_task, "blinkyBlue", led_task_func, NULL,
                 LED_TASK_PRIO, OS_WAIT_FOREVER, led_task_stack,
                 LED_STACK_SIZE);

   os_task_init(&led_task2, "blinkyRed", led_task2_func, NULL,
                 LED_TASK2_PRIO, OS_WAIT_FOREVER, led_task2_stack,
                 LED_STACK_SIZE);

    /* Initialize the OS */
    sysinit();

    while (1) {
        /* Run the event queue to process background events */
        os_eventq_run(os_eventq_dflt_get());
    }

    return rc;
}

static void
led_task_func(void *arg)
{
    /* Configure the LED GPIO as an output and HIGH (On) */
    hal_gpio_init_out(LED_2, 1);

    while (1) {
        /* Wait one second */
        os_time_delay(OS_TICKS_PER_SEC/10);

        /* Toggle the LED */
        hal_gpio_toggle(LED_2);
    }
}

static void
led_task2_func(void *arg)
{
    /* Configure the LED GPIO as an output and HIGH (On) */
    hal_gpio_init_out(LED_BLINK_PIN, 1);

    while (1) {
        /* Wait one second */
        os_time_delay(OS_TICKS_PER_SEC * 1);

        /* Toggle the LED */
        hal_gpio_toggle(LED_BLINK_PIN);
    }
}

