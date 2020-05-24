#include <linux/phonebook.h>
#include <linux/unistd.h>

int add_user(const struct user_data* data)
{
    int ret;
    asm volatile
    (
        "int $0x80"
        : "=a" (ret)
        : "0"(__NR_add_user), "b"(data)
        : "memory"
    );
    return ret;
}

int get_user(const char* surname, unsigned int surname_len, struct user_data* data)
{
    int ret;
    asm volatile
    (
        "int $0x80"
        : "=a" (ret)
        : "0"(__NR_get_user), "b"(surname), "c"(surname_len), "d"(data)
        : "memory"
    );
    return ret;
}

int del_user(const char* surname, unsigned int surname_len)
{
    int ret;
    asm volatile
    (
        "int $0x80"
        : "=a" (ret)
        : "0"(__NR_del_user), "b"(surname), "c"(surname_len)
        : "memory"
    );
    return ret;
}

int main() {
	struct user_data data = {
		.name = "John",
		.surname = "Smith",
		.phone = "1234",
		.email = "a@b.c",
		.age = 20
	};

	add_user(&data);

	return 0;
}
