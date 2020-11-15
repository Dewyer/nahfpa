#include "SvgFile.h"
#include "debugmalloc.h"

struct SvgFile
{
	FILE *file;
};

SvgFile *SvgFile_new(FILE *file)
{
	SvgFile *self = (SvgFile *) malloc(sizeof(*self));
	self->file = file;

	return self;
}

void SvgFile_free(SvgFile *self)
{
	free(self);
}
