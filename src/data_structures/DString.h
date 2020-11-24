//
// Created by barna on 10/09/2020.
//

#ifndef NAHFPA_DSTRING_H
#define NAHFPA_DSTRING_H

#include <stdbool.h>
#include <stddef.h>

typedef struct DString DString;

DString *DString_new();

DString *DString_clone(const DString *string);

DString *DString_from_CString(const char *str);

char *DString_to_CString(const DString *self);

size_t DString_len(const DString *self);

void DString_add_char(DString *self, char chr);

bool DString_eq_CString(const DString *string1, const char *string2);

bool DString_eq_DString(const DString *string1, const DString *string2);

void DString_free(DString *self);

void DString_concat(DString *self, const DString *string2);

void DString_concat_CString(DString *self, const char *string2);

char DString_char_at(const DString *self, size_t ii);

bool DString_starts_with(DString *self, char *string2);

DString *DString_substring(const DString *self, size_t start, size_t end);

#endif //NAHFPA_DSTRING_H
