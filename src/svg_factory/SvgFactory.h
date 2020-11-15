#ifndef NAHFPA_SVGFACTORY_H
#define NAHFPA_SVGFACTORY_H

typedef struct SvgFactory SvgFactory;

SvgFactory* SvgFactory_new(FILE *file);
void SvgFactory_free(SvgFactory* self);

#endif //NAHFPA_SVGFACTORY_H
