#include <linux/slab.h>
#include <linux/string.h>

#include "phonebook_impl.h"

static struct person* list_head = NULL;

struct person* get_person(char *surname) {
	struct person* current_person = list_head;
	while (current_person != NULL) {
		if (!strncmp(current_person->surname, surname, FIELD_SIZE)) {
			break;
		}
		current_person = current_person->next;
	}

	return current_person;
}

void add_person(char *name, char *surname, char* phone, char* email, unsigned char age) {
	struct person* new = kmalloc(sizeof(struct person), GFP_KERNEL);
	strncpy(new->name,    name,    FIELD_SIZE);
	strncpy(new->surname, surname, FIELD_SIZE);
	strncpy(new->phone,   phone,   FIELD_SIZE);
	strncpy(new->email,   email,   FIELD_SIZE);
	new->age = age;

	new->next = list_head;
	list_head = new;
}

void remove_person(char *surname) {
	struct person** prev_ptr = &list_head;
	while (*prev_ptr != NULL) {
		if (!strncmp((*prev_ptr)->surname, surname, FIELD_SIZE)) {
			struct person* removee = *prev_ptr;
			*prev_ptr = (*prev_ptr)->next;
			kfree(removee);
			break;
		}
		prev_ptr = &(*prev_ptr)->next;
	}
}

void persons_clear(void) {
	struct person* current_person = list_head;
	list_head = NULL;
	while (current_person != NULL) {
		struct person* tmp = current_person;
		current_person = current_person->next;
		kfree(tmp);
	}
}
