#include <expression_parser/ExpNode.h>
#include <logger/logger.h>
#include <svg_file/SvgFile.h>
#include <utils/cassert.h>
#include <assert.h>
#include <utils/geomerty.h>
#include <utils/symbols_helper.h>
#include <utils/rendering_constants.h>
#include "SvgFactory.h"
#include "debugmalloc.h"
#include "BoxNode.h"
#include "BoxModelBuilder.h"

struct SvgFactory
{
	Logger *logger;
	BoxModelBuilder *box_model_builder;
	BoxNode *box_tree_root;
	char *out_file_path;
	SvgFile *svg_file;
};

SvgFactory *SvgFactory_new(Logger *logger, ExpNode *exp_tree_root, char *out_file_path)
{
	SvgFactory *self = (SvgFactory *) malloc(sizeof(*self));
	assert(self && "Couldn't allocate SvgFactory");

	self->logger = logger;
	self->box_model_builder = BoxModelBuilder_new(logger, exp_tree_root);
	self->box_tree_root = NULL;
	self->out_file_path = out_file_path;
	self->svg_file = NULL;

	return self;
}

void SvgFactory_render_node(SvgFactory *self, BoxNode *node);

void SvgFactory_render_children(SvgFactory *self, BoxNode *node)
{
	if (node->arg1_box) {
		BoxNode_calc_global_position(node->arg1_box, node);
		SvgFactory_render_node(self, node->arg1_box);
	}

	if (node->arg2_box) {
		BoxNode_calc_global_position(node->arg2_box, node);
		SvgFactory_render_node(self, node->arg2_box);
	}

	if (node->arg3_box) {
		BoxNode_calc_global_position(node->arg3_box, node);
		SvgFactory_render_node(self, node->arg3_box);
	}

	if (node->node_list_box)
		for (size_t ii = 0; ii < node->node_list_box->item_count; ++ii) {
			BoxNode *at_box_node = List_get(node->node_list_box, ii);
			BoxNode_calc_global_position(at_box_node, node);

			SvgFactory_render_node(self, at_box_node);
		}
}

void SvgFactory_render_frac(SvgFactory *self, BoxNode *box_node)
{
	double delta_h = 0;
	if (box_node->arg1_box)
		delta_h += box_node->arg1_box->box.height + FRAC_LINE_HEIGHT / 2;

	Vector p1 = {box_node->global_pos.x, box_node->global_pos.y + delta_h};
	Vector p2 = {box_node->global_pos.x + box_node->box.width, box_node->global_pos.y + delta_h};

	SvgFile_add_line(self->svg_file, &p1, &p2);
}

void SvgFactory_render_sqrt(SvgFactory *self, BoxNode *box_node)
{
	const Vector *sqrt_off = &SQRT_BOX_DELTA;
	const double height = box_node->box.height;
	const double width = box_node->box.width;
	double sqrt_uptick = height * SQRT_TICK_HEIGHT_RATIO;
	Vector tick_u;
	Vector tick_d;
	Vector up_s;
	Vector end_s;

	tick_u.y = height - sqrt_uptick;
	tick_u.x = 0;

	tick_d.y = height;
	tick_d.x = sqrt_off->x / 2.0;

	up_s.y = sqrt_off->y / 2.0;
	up_s.x = sqrt_off->x;

	end_s.y = up_s.y;
	end_s.x = width;

	Vector_add_v(&tick_u, &box_node->global_pos);
	Vector_add_v(&tick_d, &box_node->global_pos);
	Vector_add_v(&up_s, &box_node->global_pos);
	Vector_add_v(&end_s, &box_node->global_pos);

	SvgFile_add_line(self->svg_file, &tick_u, &tick_d);
	SvgFile_add_line(self->svg_file, &tick_d, &up_s);
	SvgFile_add_line(self->svg_file, &up_s, &end_s);
}

void SvgFactory_render_sum_prod(SvgFactory *self, BoxNode *box_node)
{
	BoxNode *upper = box_node->arg1_box;
	DString *text_sub = NULL;
	Vector text_pos;
	const Size *mid_size = box_node->node->type == Sum ? &SUM_SIZE : &PROD_SIZE;

	if (upper)
		text_pos.y = upper->box.height + SUM_PROD_PADDING;

	Box_horizontal_center(&text_pos, mid_size, &box_node->box);

	if (box_node->node->type == Sum)
		text_sub = DString_from_CString("∑");
	else if (box_node->node->type == Prod)
		text_sub = DString_from_CString("∏");

	SvgFile_add_text(self->svg_file, text_sub, &text_pos);

	if (text_sub)
		DString_free(text_sub);
}

void SvgFactory_render_lim(SvgFactory *self, BoxNode *box_node)
{
	Vector pos;
	pos.y = 0;
	Box_horizontal_center(&pos, &LIM_SIZE, &box_node->box);

	DString *lim = DString_from_CString("lim");
	SvgFile_add_text(self->svg_file, lim, &pos);
	DString_free(lim);
}

void SvgFactory_render_symbol(SvgFactory *self, BoxNode *box_node)
{
	SymbolDefinitionFindResults sym_res = SymbolDefinition_get_supported_results(box_node->node->value);
	const char *sub = sym_res.is_uppercase ? sym_res.definition->uppercase_substitution
										   : sym_res.definition->substitution;

	DString *sub_ds = DString_from_CString(sub);
	Vector pos = box_node->global_pos;
	pos.y += TEXT_CORRECTION;
	SvgFile_add_text(self->svg_file, sub_ds, &pos);
	DString_free(sub_ds);
}

void SvgFactory_render_node(SvgFactory *self, BoxNode *box_node)
{
	if (DEBUG_BOXES)
		SvgFile_add_box(self->svg_file, &box_node->global_pos, &box_node->box); // Debug

	SvgFactory_render_children(self, box_node);

	if (box_node->node->type == Literal) {
		SvgFile_add_text(self->svg_file, box_node->node->value, &box_node->global_pos);
	} else if (box_node->node->type == Symbol) {
		SvgFactory_render_symbol(self, box_node);
	} else if (box_node->node->type == Frac) {
		SvgFactory_render_frac(self, box_node);
	} else if (box_node->node->type == Sqrt) {
		SvgFactory_render_sqrt(self, box_node);
	} else if (box_node->node->type == Sum || box_node->node->type == Prod) {
		SvgFactory_render_sum_prod(self, box_node);
	} else if (box_node->node->type == Lim) {
		SvgFactory_render_lim(self, box_node);
	}
}

void SvgFactory_create(SvgFactory *self)
{
	BoxNode *box_root = BoxModelBuilder_build(self->box_model_builder);
	self->box_tree_root = box_root;
	Logger_log(self->logger, LogInfo, "Box model dimensions: w: %f, h: %f", box_root->box.width, box_root->box.height);
	Logger_log(self->logger, LogInfo, "STEP 3. Box Model built");

	FILE *file = fopen(self->out_file_path, "w");
	cassert(self->logger, !!file, "Couldn't open output file to write to.");
	self->svg_file = SvgFile_new(file, &box_root->box);

	SvgFactory_render_node(self, box_root);
	Logger_log(self->logger, LogInfo, "STEP 4. Rendering done.");
}

void SvgFactory_free(SvgFactory *self)
{
	if (self->box_tree_root)
		BoxNode_free(self->box_tree_root);

	BoxModelBuilder_free(self->box_model_builder);

	if (self->svg_file)
		SvgFile_close(self->svg_file);

	free(self);
}
