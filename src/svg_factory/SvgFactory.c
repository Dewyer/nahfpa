#include <expression_parser/ExpNode.h>
#include <logger/logger.h>
#include <svg_file/SvgFile.h>
#include <utils/cassert.h>
#include "SvgFactory.h"
#include "debugmalloc.h"

struct SvgFactory
{
	Logger *logger;
	ExpNode *exp_tree_root;
	char *out_file_path;

	SvgFile *svg_file;
};

SvgFactory *SvgFactory_new(Logger *logger, ExpNode *exp_tree_root, char *out_file_path)
{
	SvgFactory *self = (SvgFactory *) malloc(sizeof(*self));
	self->logger = logger;
	self->exp_tree_root = exp_tree_root;
	self->out_file_path = out_file_path;

	FILE *file = fopen(self->out_file_path, "w");
	cassert(self->logger, !!file, "Couldn't open output file to write to.");

	self->svg_file = SvgFile_new(file);

	return self;
}

void SvgFactory_create()
{

}

void SvgFactory_free(SvgFactory *self)
{
	SvgFile_free(self->svg_file);
	free(self);
}
