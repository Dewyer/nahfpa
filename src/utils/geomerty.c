//
// Created by barna on 15/11/2020.
//

#include "geomerty.h"

void Size_add_v(Size *size, const Vector *v) {
	size->width += v->x;
	size->height += v->y;
}

void Size_add_s(Size *size, const Size *size2) {
	size->width += size2->width;
	size->height += size2->height;
}

void Vector_add_v(Vector *vector, const Vector *v2) {
	vector->x += v2->x;
	vector->y += v2->y;
}

void Vector_add(Vector *vector, double x, double y) {
	vector->x += x;
	vector->y += y;
}

void Vector_negate(const Vector *vector, Vector *target) {
	target->x = -vector->x;
	target->y = -vector->y;
}

void Box_horizontal_center(Vector *pos, const Size *box, const Size *container) {
	pos->x = container->width / 2 - box->width / 2;
}

void Box_vertical_center(Vector *pos, const Size *box, const Size *container) {
	pos->y = container->height / 2 - box->height / 2;
}

void Box_align_to_base_line(Vector *pos, const Size *box, const Size *container) {
	pos->y = container->height - box->height;
}

double double_max(double aa, double bb) {
	return aa > bb ? aa : bb;
}

