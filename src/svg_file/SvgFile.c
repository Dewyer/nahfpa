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
		".text { font-family: 'Times new Roman', serif; fill: black; font-size: 30px; letter-spacing: 1px;}"
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

DString *SvgFile_escape_text(const DString *txt)
{
	DString *new_str = DString_new();

	for (int ii = 0; ii < DString_len(txt); ++ii) {
		if (DString_char_at(txt, ii) == '&')
			DString_concat_CString(new_str, "&amp;");
		else if (DString_char_at(txt, ii) == '<')
			DString_concat_CString(new_str, "&lt;");
		else if (DString_char_at(txt, ii) == '>')
			DString_concat_CString(new_str, "&gt;");
		else if (DString_char_at(txt, ii) == '"')
			DString_concat_CString(new_str, "&quot;");
		else if (DString_char_at(txt, ii) == '\'')
			DString_concat_CString(new_str, "&#39;");
		else
			DString_add_char(new_str, DString_char_at(txt, ii));
	}

	return new_str;
}

void SvgFile_add_text(SvgFile *self, const DString *text, const Vector *pos)
{
	DString *escp_txt = SvgFile_escape_text(text);
	fprintf(self->file, "\t<text class=\"text\" x=\"%f\" y=\"%f\">%s</text>\n", pos->x,
			pos->y + TEXT_HEIGHT - TEXT_CORRECTION,
			DString_to_CString(escp_txt));

	DString_free(escp_txt);
}

void SvgFile_add_line(SvgFile *self, const Vector *p1, const Vector *p2)
{
	fprintf(self->file, "\t<line class=\"line\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />\n", p1->x, p1->y, p2->x,
			p2->y);
}

void SvgFile_add_box(SvgFile *self, const Vector *pp, const Size *size)
{
	fprintf(self->file, "\t<rect class=\"box\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />\n", pp->x, pp->y,
			size->with,
			size->height);
}

void SvgFile_close(SvgFile *self)
{
	fprintf(self->file, "</svg>");
	fclose(self->file);
	free(self);
}
