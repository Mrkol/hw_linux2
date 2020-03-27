#include <linux/phonebook.h>
#include <linux/syscalls.h>


SYSCALL_DEFINE3(sys_get_user,
				const char __user *, surname,
				unsigned int, len,
				struct user_data __user *, output_data) {


	return 0;
}

SYSCALL_DEFINE1(sys_add_user,
				struct user_data *, input_data) {



	return 0;
}

SYSCALL_DEFINE2(sys_del_user,
				const char __user *, surname,
				unsigned int, len) {


	return 0;
}
