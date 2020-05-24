#include <stdio.h>
#include <linux/phonebook.h>
#include <linux/unistd.h>
#include <unistd.h>

int main() {
	struct user_data data = {
		.name = "John",
		.surname = "Smith",
		.phone = "1234",
		.email = "a@b.c",
		.age = 20
	};

	printf("adding\n");
	syscall(__NR_add_user, &data);

	return 0;
}
