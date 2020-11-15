#ifndef NAHFPA_SVGFACTORY_H
#define NAHFPA_SVGFACTORY_H

#include "expression_parser/ExpNode.h"

typedef struct SvgFactory SvgFactory;

SvgFactory *SvgFactory_new(Logger *logger, ExpNode *exp_tree_root, char *out_file_path);

void SvgFactory_create(SvgFactory *self);

void SvgFactory_free(SvgFactory *self);

#endif //NAHFPA_SVGFACTORY_H
