//
// Created by barna on 15/11/2020.
//

#ifndef NAHFPA_GEOMERTY_H
#define NAHFPA_GEOMERTY_H

typedef struct Vector
{
	double x;
	double y;
} Vector;

typedef struct Size
{
	double with;
	double height;
} Size;

void Size_add_v(Size *size, const Vector *v);

void Size_add_s(Size *size, const Size *size2);

void Box_horizontal_center(Vector *pos, const Size *box, const Size *container);

void Box_vertical_center(Vector *pos, const Size *box, const Size *container);

void Vector_add_v(Vector *vector, const Vector *vector2);

double double_max(double aa, double bb);

#endif //NAHFPA_GEOMERTY_H