#include <stdio.h>
#include "data_structures/DString.h"
#include "expression_parser/ExpParser.h"

int main()
{
    printf("Hello, World!\n");

    DString* latex = DString_from_CString("\\frac{x + 10}{20} = 42");
    ExpParser* parser = ExpParser_new(latex);

    ExpParser_parse(parser);

    ExpParser_free(parser);
    return 0;
}
