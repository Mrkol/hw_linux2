#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/unistd.h>

#include "phonebook_device.h"
#include "phonebook_impl.h"

#define DEVICE_NAME "phonebook"

static unsigned int major_num;
static int device_open_count = 0;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

struct file_operations file_ops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.owner = THIS_MODULE
};


int phonebook_device_init(void) {
	major_num = __register_chrdev(0, 0, 1, DEVICE_NAME, &file_ops);

	if (major_num < 0) {
		printk(KERN_ALERT "Could not register device: %d\n", major_num);
		return major_num;
	}

	printk(KERN_INFO "Phonebook created with device number %d\n", major_num);
	return 0;
}

void phonebook_device_exit(void) {
	__unregister_chrdev(major_num, 0, 1, DEVICE_NAME);
}

static struct person* answer = NULL;

static ssize_t device_read(struct file *flip, char __user *buffer,
	size_t len, loff_t *offset) {

	if (!answer) {
		return 0;
	}

	// definitely can fit the person struct
	char kbuffer[512];
	char* last_free = kbuffer;
	*last_free = 0;

#define DUMP_FIELD(field) strncat(last_free, answer->field, FIELD_SIZE);\
last_free += strnlen(answer->field, FIELD_SIZE);\
*last_free = '\t'; ++last_free; *last_free = 0;

	DUMP_FIELD(name);
	DUMP_FIELD(surname);
	DUMP_FIELD(phone);
	DUMP_FIELD(email);

#undef DUMP_FIELD

	int ret = sprintf(last_free, "%d", answer->age);
	if (ret < 0) {
		return -EINVAL;
	}

	last_free += ret;
	*last_free = '\n';
	++last_free;
	*last_free = 0;

	size_t size = last_free - kbuffer;

	if (len < size) {
		return -EINVAL;
	}

	answer = NULL;
	return size - copy_to_user(buffer, kbuffer, size);
}

static ssize_t device_write(struct file *flip,
	const char __user *buffer, size_t len, loff_t *offset) {

	const size_t BUFFER_SIZE = 512;
	char kbuffer[BUFFER_SIZE];

	copy_from_user(kbuffer, buffer, min(BUFFER_SIZE - 1, len));
	// turn it into a c-str
	kbuffer[BUFFER_SIZE - 1] = 0;

#define PARSE_FIELD(start, end, sep) char* end = strchr((start), (sep));\
if ((end) == NULL) { return -EINVAL; }\
*end = 0;

	PARSE_FIELD(kbuffer, command_end, '\t');

	if (!strcmp(kbuffer, "add")) {
		PARSE_FIELD(command_end + 1, name_end,    '\t');
		PARSE_FIELD(name_end    + 1, surname_end, '\t');
		PARSE_FIELD(surname_end + 1, phone_end,   '\t');
		PARSE_FIELD(phone_end   + 1, email_end,   '\t');
		PARSE_FIELD(email_end   + 1, age_end,     '\n');

		u8 age = 0;
		if (kstrtou8(email_end + 1, 10, &age)) {
			return -EINVAL;
		}

		add_person(
			command_end + 1,
			name_end + 1,
			surname_end + 1,
			phone_end + 1,
			age);

		return age_end - kbuffer + 1;
	}

	PARSE_FIELD(command_end + 1, surname_end, '\n');

	if (!strcmp(kbuffer, "get")) {
		answer = get_person(command_end + 1);
		return surname_end - kbuffer + 1;
	} else if (!strcmp(kbuffer, "del")) {
		remove_person(command_end + 1);
		return surname_end - kbuffer + 1;
	}

#undef PARSE_FIELD

	return -EINVAL;
}

static int device_open(struct inode *inode, struct file *file) {
	if (device_open_count) {
		return -EBUSY;
	}
	device_open_count++;
	return 0;
}

static int device_release(struct inode *inode, struct file *file) {
	device_open_count--;
	return 0;
}
