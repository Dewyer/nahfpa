#include <stdlib.h>
#include <logger/logger.h>
#include "NahfpaCli.h"
#include "data_structures/DString.h"
#include "expression_parser/ExpParser.h"

struct NahfpaCli
{
	int arg_count;
	char** args;

	Logger* logger;
};

NahfpaCli *NahfpaCli_new(int argc, char *argv[])
{
	NahfpaCli *self = (NahfpaCli *) malloc(sizeof(self));
	self->arg_count = argc;
	self->args = argv;

	self->logger = Logger_new(LogInfo, NULL);

	return self;
}

void NahfpaCli_compile(NahfpaCli* self)
{
	Logger_log(self->logger, LogInfo, "NAPFHA Compiling!");
	Logger_log(self->logger, LogWarning, "NAPFHA warn!");
	Logger_log(self->logger, LogError, "NAPFHA err! %d", 200);

	DString* latex = DString_from_CString("\\frac{x + 10}{20} = 42");
	ExpParser* parser = ExpParser_new(latex);

	ExpParser_parse(parser);

	ExpParser_free(parser);
}

void NahfpaCli_free(NahfpaCli *self)
{
	Logger_free(self->logger);
	free(self);
}
