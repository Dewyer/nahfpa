#include <stdio.h>
#include <nahfpa_cli/NahfpaCli.h>

int main(int argc, char *argv[])
{
	NahfpaCli* cli = NahfpaCli_new(argc, argv);
	NahfpaCli_compile(cli);

	NahfpaCli_free(cli);
    return 0;
}
