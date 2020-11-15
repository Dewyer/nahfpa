//
// Created by barna on 15/11/2020.
//

#include "geomerty.h"

void Size_add_v(Size *size, const Vector *v)
{
	size->with += v->x;
	size->height += v->y;
}

void Size_add_s(Size *size, const Size *size2)
{
	size->with += size2->with;
	size->height += size2->height;
}

void Vector_add_v(Vector *vector, const Vector *v2)
{
	vector->x += v2->x;
	vector->y += v2->y;
}

void Box_horizontal_center(Vector *pos, const Size *box, const Size *container)
{
	pos->x = container->with / 2 - box->with / 2;
}

double double_max(double aa, double bb)
{
	return aa > bb ? aa : bb;
}

