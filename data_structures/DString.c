//
// Created by barna on 10/09/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "DString.h"
#include <string.h>
#include <assert.h>

struct DString
{
    size_t capacity;
    char* data;
};

DString* DString_new()
{
    DString* new_ptr = malloc(sizeof(*new_ptr));
    new_ptr->capacity = 0;
    new_ptr->data = NULL;

    return new_ptr;
}

DString* DString_from_CString(char* str)
{
    DString* ds = DString_new();
    ds->capacity = strlen(str);
    ds->data = calloc(ds->capacity+1,sizeof(char));
    strcpy(ds->data,str);
    assert(ds->data && "DString couldn't alloc");

    return ds;
}

char* DString_to_CString(DString* self)
{
    return self->data;
}
void DString_free(DString* self)
{
    free(self->data);
    free(self);
}
