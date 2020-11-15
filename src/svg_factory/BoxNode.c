//
// Created by barna on 15/11/2020.
//

#include "BoxNode.h"
#include "debugmalloc.h"

BoxNode *BoxNode_new(ExpNode *node)
{
	BoxNode *self = (BoxNode *) malloc(sizeof(*self));
	self->node = node;
	self->offset.x = 0;
	self->offset.y = 0;
	self->box.height = 0;
	self->box.with = 0;

	self->arg1_box = NULL;
	self->arg2_box = NULL;
	self->node_list_box = NULL;

	return self;
}

void BoxNode_free(BoxNode *self)
{
	if (self->arg1_box)
		BoxNode_free(self->arg1_box);
	if (self->arg2_box)
		BoxNode_free(self->arg2_box);
	if (self->node_list_box)
		List_free_2D(self->node_list_box, (void (*)(void *)) BoxNode_free);

	free(self);
}
