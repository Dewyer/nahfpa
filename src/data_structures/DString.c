//
// Created by barna on 10/09/2020.
//

#include <string.h>
#include <assert.h>
#include "DString.h"
#include "debugmalloc.h"

struct DString
{
    size_t capacity;
    char* data;
};

DString* DString_new()
{
    DString* new_ptr = (DString*) malloc(sizeof(*new_ptr));
    new_ptr->capacity = 0;
    new_ptr->data = NULL;

    return new_ptr;
}

DString* DString_from_CString(char* str)
{
    DString* ds = DString_new();
    ds->capacity = strlen(str);
    ds->data = (char*) calloc(ds->capacity+1,sizeof(char));
    strcpy(ds->data,str);
    assert(ds->data && "DString couldn't alloc");

    return ds;
}

char* DString_to_CString(DString* self)
{
    return self->data;
}

int DString_len(const DString* self)
{
	if (self->data)
		return (int)strlen(self->data);

	return 0;
}

void DString_add_char(DString* self, char chr)
{
	char* new_ptr = (char*) calloc(self->capacity + 2,sizeof(*new_ptr));
	strcpy(new_ptr,self->data);

	char temp[2] = {chr,'\0'};
	strcat(new_ptr,temp);

	free(self->data);
	self->data = new_ptr;
	self->capacity += 2;
}

void DString_free(DString* self)
{
    free(self->data);
    free(self);
}

DString* DString_clone(DString* string)
{
	return DString_from_CString(DString_to_CString(string));
}
