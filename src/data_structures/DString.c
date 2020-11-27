//
// Created by barna on 10/09/2020.
//

#include <string.h>
#include <assert.h>
#include "DString.h"
#include "debugmalloc.h"

struct DString {
	size_t capacity;
	char *data;
};

void DString_resize(DString *self, size_t new_size) {
	char *new_ptr = (char *) calloc(new_size + 1, sizeof(*new_ptr));
	assert(new_ptr && "DString couldn't alloc");

	if (self->data) {
		strcpy(new_ptr, self->data);
		free(self->data);
	} else
		new_ptr[0] = '\0';

	self->data = new_ptr;
	self->capacity = new_size;
}

DString *DString_new() {
	DString *new_ptr = (DString *) malloc(sizeof(*new_ptr));
	assert(new_ptr && "DString couldn't alloc");

	new_ptr->capacity = 0;
	new_ptr->data = NULL;

	return new_ptr;
}

DString *DString_from_CString(const char *str) {
	DString *ds = DString_new();
	ds->capacity = strlen(str);
	ds->data = (char *) calloc(ds->capacity + 1, sizeof(char));
	assert(ds->data && "DString couldn't alloc");

	strcpy(ds->data, str);

	return ds;
}

char *DString_to_CString(const DString *self) {
	return self->data;
}

size_t DString_len(const DString *self) {
	if (self->data)
		return (int) strlen(self->data);

	return 0;
}

void DString_add_char(DString *self, char chr) {
	DString_resize(self, self->capacity + 1);
	char temp[2] = {chr, '\0'};
	strcat(self->data, temp);
}

void DString_free(DString *self) {
	if (self->data)
		free(self->data);

	free(self);
}

DString *DString_clone(const DString *string) {
	return DString_from_CString(DString_to_CString(string));
}

bool DString_eq_CString(const DString *string1, const char *string2) {
	return strcmp(string1->data, string2) == 0;
}

bool DString_eq_DString(const DString *string1, const DString *string2) {
	return DString_eq_CString(string1, DString_to_CString(string2));
}

void DString_concat(DString *self, const DString *string2) {
	DString_resize(self, self->capacity + string2->capacity);
	strcat(self->data, string2->data);
}

void DString_concat_CString(DString *self, const char *string2) {
	DString_resize(self, self->capacity + strlen(string2));
	strcat(self->data, string2);
}

char DString_char_at(const DString *self, size_t ii) {
	assert((ii < (size_t) DString_len(self)) && "Out of bounds char index for DString");
	return self->data[ii];
}

bool DString_starts_with(DString *self, char *string2) {
	const size_t len = strlen(self->data);
	const size_t len2 = strlen(string2);

	if (len2 > len)
		return false;

	for (size_t ii = 0; ii < len; ++ii) {
		if (ii >= len2)
			break;
		if (self->data[ii] != string2[ii])
			return false;
	}

	return true;
}

DString *DString_substring(const DString *self, size_t start, size_t end) {
	assert((start < end) && "DString substring start over end");
	assert((end < DString_len(self)) && "DString substring end over end of string");

	DString *new_string = DString_new();
	size_t copy_len = end - start + 1;
	DString_resize(new_string, copy_len);

	strncpy(new_string->data, self->data + start, copy_len);
	new_string->data[copy_len] = '\0';

	return new_string;
}
