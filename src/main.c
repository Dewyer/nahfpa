#include <stdio.h>
#include <nahfpa_cli/NahfpaCli.h>
#include "debugmalloc.h"

int main(int argc, char *argv[])
{
	NahfpaCli* cli = NahfpaCli_new(argc, argv);
	NahfpaCli_execute(cli);

	NahfpaCli_free(cli);
    return 0;
}
