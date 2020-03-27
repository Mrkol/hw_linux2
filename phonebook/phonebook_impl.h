#ifndef PHONEBOOK_IMPL_H
#define PHONEBOOK_IMPL_H

#include <linux/hashtable.h>

#define FIELD_SIZE 32
#define PERSON_SIZE sizeof(struct person)

struct person {
	char name[FIELD_SIZE];
	char surname[FIELD_SIZE];
	char phone[FIELD_SIZE];
	char email[FIELD_SIZE];
	unsigned char age;
	struct person* next;
};

struct person* get_person(char *surname);
void add_person(char *name, char *surname, char* phone, char* email, unsigned char age);
void remove_person(char *surname);
void persons_clear(void);

#endif // PHONEBOOK_IMPL_H
