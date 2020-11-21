#ifndef NAHFPA_AMFTEXTWIDTH_H
#define NAHFPA_AMFTEXTWIDTH_H

typedef struct AmfTextWidth
{
	int character_map[256];
} AmfTextWidth;

void AmfTextWidth_init(AmfTextWidth *self);

double AmfTextWidth_char_width_calc(const AmfTextWidth *self, char cc, double font_size);

#endif //NAHFPA_AMFTEXTWIDTH_H
