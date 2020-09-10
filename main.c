#include <stdio.h>
#include "data_structures/DString.h"
#include "expression_parser/ExpParser.h"

int main()
{
    printf("Hello, World!\n");

    DString* latex = DString_from_CString("x + 10 = 42");
    ExpParser* parser = ExpParser_new(latex);

    ExpParser_free(parser);
    return 0;
}
