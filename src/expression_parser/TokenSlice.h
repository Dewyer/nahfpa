#ifndef NAHFPA_TOKENSLICE_H
#define NAHFPA_TOKENSLICE_H

#include <stddef.h>

typedef struct TokenSlice {
	size_t start;
	size_t end;
} TokenSlice;

TokenSlice *TokenSlice_new(size_t start, size_t end);

TokenSlice *TokenSlice_shrink_clone(TokenSlice *slice);

void TokenSlice_free(TokenSlice *self);

#endif //NAHFPA_TOKENSLICE_H
