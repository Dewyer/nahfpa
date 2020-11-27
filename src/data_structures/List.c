//
// Created by Rátki Barnabás on 2020. 09. 07..
//
#include "List.h"
#include "debugmalloc.h"

List *List_new() {
	List *ll = (List *) malloc(sizeof(*ll));
	ll->head = NULL;
	ll->item_count = 0;
	return ll;
}

void List_push(List *self, void *item) {
	ListItem *last_item_ptr = self->head;

	for (int ii = 1; (size_t) ii < self->item_count; ii++) {
		last_item_ptr = last_item_ptr->next_ptr;
	}

	ListItem *new_item = (ListItem *) malloc(sizeof(*new_item));
	new_item->next_ptr = NULL;
	new_item->last_ptr = NULL;
	new_item->item_data = item;

	if (last_item_ptr != NULL) {
		last_item_ptr->next_ptr = new_item;
		new_item->last_ptr = last_item_ptr;
	} else {
		self->head = new_item;
	}

	self->item_count++;
}

void *List_get(const List *self, size_t index) {
	if (index >= self->item_count) {
		return NULL;
	}
	ListItem *at_item_ptr = self->head;

	for (size_t ii = 0; ii < index; ii++) {
		at_item_ptr = at_item_ptr->next_ptr;
	}

	return at_item_ptr->item_data;
}

void List_free_2D(List *self, void (*free_item)(void *)) {
	if (self->item_count == 0) {
		free(self);
		return;
	}

	ListItem *ptr_i = self->head;
	while (ptr_i != NULL) {
		ListItem *saved_ptr = ptr_i;
		ptr_i = ptr_i->next_ptr;

		if (free_item)
			free_item(saved_ptr->item_data);

		free(saved_ptr);
	}
	free(self);
}

void List_free(List *self) {
	List_free_2D(self, NULL);
}
