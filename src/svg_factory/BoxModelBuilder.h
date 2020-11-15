//
// Created by barna on 15/11/2020.
//

#ifndef NAHFPA_BOXMODELBUILDER_H
#define NAHFPA_BOXMODELBUILDER_H

#include <expression_parser/ExpNode.h>
#include <logger/logger.h>
#include "BoxNode.h"

typedef struct BoxModelBuilder BoxModelBuilder;

BoxModelBuilder *BoxModelBuilder_new(Logger *logger, ExpNode *exp_tree_root);

BoxNode *BoxModelBuilder_build(BoxModelBuilder *self);

void BoxModelBuilder_free(BoxModelBuilder *self);

#endif //NAHFPA_BOXMODELBUILDER_H
