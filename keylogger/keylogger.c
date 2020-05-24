#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Sandu");
MODULE_DESCRIPTION("ps/2 keylogger");
MODULE_VERSION("0.01");


atomic_long_t keypress_counter = { .counter = 0 };

struct tasklet_struct bottom_tasklet;
struct task_struct* logger_thread;

void bottom(unsigned long unused) {
	atomic_long_inc(&keypress_counter);
}

irqreturn_t top(int irq, void* dev_id) {
	tasklet_schedule(&bottom_tasklet);
	return IRQ_HANDLED;
}

int logger_main(void* data) {
    while (!kthread_should_stop()) {
    	long long last = atomic_long_fetch_and(0, &keypress_counter);
        printk(KERN_INFO "%lld keypresses in the last minute\n", last);
        msleep(1000 * 60);
    }
    return 0;
}

static int __init phonebook_init(void) {
	// TODO: error handling
    tasklet_init(&bottom_tasklet, bottom, 0);

	request_irq(1, top, IRQF_SHARED,
		"keypress_interrput", &bottom_tasklet);

	logger_thread = kthread_create(logger_main, NULL, "logger_thread");
	wake_up_process(logger_thread);

	return 0;
}

static void __exit phonebook_exit(void) {
	free_irq(1, &bottom_tasklet);
	tasklet_kill(&bottom_tasklet);
	kthread_stop(logger_thread);
}

module_init(phonebook_init);
module_exit(phonebook_exit);
