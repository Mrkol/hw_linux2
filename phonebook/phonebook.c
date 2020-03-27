#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "phonebook_device.h"
#include "phonebook_impl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Sandu");
MODULE_DESCRIPTION("Phonebook kernel module thing");
MODULE_VERSION("0.01");

static int __init phonebook_init(void) {
	int error = 0;
	if (!(error = phonebook_device_init())) {
		return error;
	}

	return 0;
}

static void __exit phonebook_exit(void) {
	phonebook_device_exit();
	persons_clear();
}

module_init(phonebook_init);
module_exit(phonebook_exit);
