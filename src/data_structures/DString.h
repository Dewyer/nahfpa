//
// Created by barna on 10/09/2020.
//

#ifndef NAHFPA_DSTRING_H
#define NAHFPA_DSTRING_H

#include <stdbool.h>

typedef struct DString DString;

DString *DString_new();

DString *DString_clone(const DString *string);

DString *DString_from_CString(const char *str);

char *DString_to_CString(const DString *self);

int DString_len(const DString *self);

void DString_add_char(DString *self, char chr);

bool DString_eq_CString(const DString *string1, const char *string2);

void DString_free(DString *self);

void DString_concat(DString *self, const DString *string2);

bool DString_starts_with(DString *self, char *string2);

#endif //NAHFPA_DSTRING_H
