#include <utils/geomerty.h>
#include <utils/rendering_constants.h>
#include "stdio.h"
#include "SvgFile.h"
#include "debugmalloc.h"

struct SvgFile
{
	FILE *file;
};

const char *STYLE =
		".text { font-family: monospace; fill: black; font-size: 30px; }"
		".line { stroke: black; }"
		".box { stroke: red; fill: rgba(0,0,0,0) }";

SvgFile *SvgFile_new(FILE *file, Size *view_port)
{
	SvgFile *self = (SvgFile *) malloc(sizeof(*self));
	self->file = file;

	fprintf(file,
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n<svg width=\"%f\" height=\"%f\" xmlns=\"http://www.w3.org/2000/svg\">\n"
			"<style>%s</style>\n",
			view_port->with, view_port->height, STYLE);
	return self;
}

void SvgFile_add_text(SvgFile *self,const DString *text,const Vector *pos)
{
	fprintf(self->file, "\t<text class=\"text\" x=\"%f\" y=\"%f\">%s</text>\n", pos->x, pos->y+TEXT_HEIGHT-TEXT_CORRECTION,
			DString_to_CString(text));
}

void SvgFile_add_line(SvgFile *self,const Vector *p1,const Vector *p2)
{
	fprintf(self->file, "\t<line class=\"line\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />\n", p1->x, p1->y, p2->x,
			p2->y);
}

void SvgFile_add_box(SvgFile *self,const Vector *pp,const Size *size)
{
	fprintf(self->file, "\t<rect class=\"box\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", pp->x, pp->y, size->with,
			size->height);
}

void SvgFile_close(SvgFile *self)
{
	fprintf(self->file, "</svg>");
	fclose(self->file);
	free(self);
}
