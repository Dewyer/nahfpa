#ifndef NAHFPA_SVGFILE_H
#define NAHFPA_SVGFILE_H

#include <data_structures/DString.h>
#include "stdio.h"
#include "utils/geomerty.h"

typedef struct SvgFile SvgFile;

SvgFile *SvgFile_new(FILE *file, Size *view_port);

void SvgFile_add_text(SvgFile *self, const DString *text, const Vector *pos);

void SvgFile_add_path(SvgFile *self, const char* format, ...);

void SvgFile_add_line(SvgFile *self, const Vector *p1, const Vector *p2);

void SvgFile_add_box(SvgFile *self, const Vector *pp, const Size *size);

void SvgFile_close(SvgFile *self);

#endif //NAHFPA_SVGFILE_H
