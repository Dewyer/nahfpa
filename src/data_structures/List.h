//
// Created by Rátki Barnabás on 2020. 09. 07..
//

#ifndef UNTITLED_LINKEDLIST_H
#define UNTITLED_LINKEDLIST_H

#include <stddef.h>

#define ListG(type) List

typedef struct List {
	size_t item_count;
	struct ListItem *head;
} List;

typedef struct ListItem {
	void *item_data;
	struct ListItem *next_ptr;
	struct ListItem *last_ptr;
} ListItem;

List *List_new();

void List_push(List *self, void *item);

void *List_get(const List *self, size_t index);

void List_free_2D(List *self, void (*free_item)(void *));

void List_free(List *self);

#endif //UNTITLED_LINKEDLIST_H
