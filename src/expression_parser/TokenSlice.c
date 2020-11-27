#include "TokenSlice.h"
#include "debugmalloc.h"

TokenSlice *TokenSlice_new(size_t start, size_t end) {
	TokenSlice *self = (TokenSlice *) malloc(sizeof(*self));
	self->start = start;
	self->end = end;
	return self;
}

TokenSlice *TokenSlice_shrink_clone(TokenSlice *slice) {
	return TokenSlice_new(slice->start + 1, slice->end - 1);
}

void TokenSlice_free(TokenSlice *self) {
	free(self);
}
