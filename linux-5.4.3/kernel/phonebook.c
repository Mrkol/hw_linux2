#include <uapi/linux/phonebook.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>


SYSCALL_DEFINE3(sys_get_user,
				const char __user *, surname,
				unsigned int, len,
				struct user_data __user *, output_data) {

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);


	int fd = ksys_open("/dev/phonebook", O_RDWR, 0);
	{
		char buffer[64];
		strcpy(buffer, "get\t");
		copy_from_user(buffer + 4, surname, min((unsigned int)USER_DATA_FIELD_SIZE, len));

		ksys_write(fd, buffer, 64);
	}

	{
		struct user_data data;
		copy_from_user(&data, output_data, sizeof(struct user_data));

		char buffer[512];
		ksys_read(fd, buffer, 512);
		buffer[511] = 0;


#define PARSE_FIELD(start, end, sep, to) char* end = strchr((start), (sep));\
if ((end) == NULL) { return -EINVAL; }\
*end = 0;\
copy_to_user((to), (start),\
	min((unsigned long)USER_DATA_FIELD_SIZE, (unsigned long)(end - (start))));

		PARSE_FIELD(buffer,          name_end,    '\t', data.name);
		PARSE_FIELD(name_end + 1,    surname_end, '\t', data.surname);
		PARSE_FIELD(surname_end + 1, phone_end,   '\t', data.phone);
		PARSE_FIELD(phone_end + 1,   email_end,   '\t', data.email);

#undef PARSE_FIELD

		u8 age = 0;
		kstrtou8(email_end + 1, 10, &age);

		copy_to_user(&data.age, &age, sizeof(u8));
	}

	ksys_close(fd);

	set_fs(old_fs);

	return 0;
}

SYSCALL_DEFINE1(sys_add_user,
				struct user_data __user *, input_data) {

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);


	int fd = ksys_open("/dev/phonebook", O_WRONLY, 0);

	struct user_data data;
	copy_from_user(&data, input_data, sizeof(struct user_data));

	char buffer[512];
	buffer[511] = 0;
	char* ptr = buffer;

#define ADD_STR(str, len) strncpy(ptr, (str), (len));\
ptr += strnlen(ptr, 512);

	ADD_STR("add\t", 4);
	ADD_STR(data.name, USER_DATA_FIELD_SIZE);
	ADD_STR("\t", 1);
	ADD_STR(data.surname, USER_DATA_FIELD_SIZE);
	ADD_STR("\t", 1);
	ADD_STR(data.phone, USER_DATA_FIELD_SIZE);
	ADD_STR("\t", 1);
	ADD_STR(data.email, USER_DATA_FIELD_SIZE);
	ADD_STR("\t", 1);
	sprintf(ptr, "%d", data.age);
	ptr += strnlen(ptr, 512);
	ADD_STR("\n", 1);

#undef ADD_STR

	ksys_write(fd, buffer, strnlen(buffer, 512));

	ksys_close(fd);

	set_fs(old_fs);

	return 0;
}

SYSCALL_DEFINE2(sys_del_user,
				const char __user *, surname,
				unsigned int, len) {

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	int fd = ksys_open("/dev/phonebook", O_RDWR, 0);

	char buffer[64];
	strcpy(buffer, "del\t");
	copy_from_user(buffer + 4, surname, min((unsigned int)USER_DATA_FIELD_SIZE, len));

	ksys_write(fd, buffer, 64);

	ksys_close(fd);

	set_fs(old_fs);

	return 0;
}
