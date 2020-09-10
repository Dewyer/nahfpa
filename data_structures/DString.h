//
// Created by barna on 10/09/2020.
//

#ifndef NAHFPA_DSTRING_H
#define NAHFPA_DSTRING_H

typedef struct DString DString;

DString* DString_new();
DString* DString_from_CString(char* str);
char* DString_to_CString(DString* self);
void DString_free(DString* self);

#endif //NAHFPA_DSTRING_H
