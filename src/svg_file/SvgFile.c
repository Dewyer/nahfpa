#include "stdio.h"
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

void SvgFile_close(SvgFile *self)
{
	fclose(self->file);
	free(self);
}
