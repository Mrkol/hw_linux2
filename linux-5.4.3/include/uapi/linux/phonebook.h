#ifndef _LINUX_PHONEBOOK_H
#define _LINUX_PHONEBOOK_H

#include <linux/types.h>

#define USER_DATA_FIELD_SIZE 32

struct user_data {
	char name    [USER_DATA_FIELD_SIZE];
	char surname [USER_DATA_FIELD_SIZE];
	char phone   [USER_DATA_FIELD_SIZE];
	char email   [USER_DATA_FIELD_SIZE];
	unsigned char age;
};

#endif // _LINUX_PHONEBOOK_H
