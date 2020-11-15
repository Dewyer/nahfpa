#ifndef NAHFPA_SVGFILE_H
#define NAHFPA_SVGFILE_H

typedef struct SvgFile SvgFile;

SvgFile *SvgFile_new();

void SvgFile_close(SvgFile *self);

#endif //NAHFPA_SVGFILE_H
