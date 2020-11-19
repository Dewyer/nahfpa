//
// Created by barna on 19/11/2020.
//

#include <stddef.h>
#include "amf_text_width.h"
#include "debugmalloc.h"

typedef struct AmfCharacterDefinition
{
	char character;
	int wx;
} AmfCharacterDefinition;

static AmfCharacterDefinition SUPPORTED_AMF_CHARACTERS[] = {
		{'W',944},
		{'&',921},
		{'M',889},
		{'%',833},
		{'&',778},
		{'m',778},
		{'A',722},
		{'D',722},
		{'G',722},
		{'H',722},
		{'K',722},
		{'N',722},
		{'O',722},
		{'Q',722},
		{'U',722},
		{'V',722},
		{'X',722},
		{'Y',722},
		{'w',722},
		{'B',667},
		{'C',667},
		{'R',667},
		{'E',611},
		{'L',611},
		{'T',611},
		{'Z',611},
		{'+',564},
		{'<',564},
		{'=',564},
		{'>',564},
		{'F',556},
		{'P',556},
		{'S',556},
		{'~',541},
		{'$',500},
		{'\'',500},
		{'0',500},
		{'1',500},
		{'2',500},
		{'3',500},
		{'4',500},
		{'5',500},
		{'6',500},
		{'7',500},
		{'8',500},
		{'9',500},
		{'_',500},
		{'b',500},
		{'d',500},
		{'g',500},
		{'h',500},
		{'k',500},
		{'n',500},
		{'o',500},
		{'p',500},
		{'q',500},
		{'u',500},
		{'v',500},
		{'x',500},
		{'y',500},
		{'a',444},
		{'c',444},
		{'e',444},
		{'z',444},
		{'J',389},
		{'s',389},
		{'(',333},
		{')',333},
		{'-',333},
		{'I',333},
		{'[',333},
		{']',333},
		{'{',340},
		{'}',340},
		{'f',333},
		{'r',333},
		{'!',333},
		{'`',333},
		{'~',333},
		{'/',278},
		{'.',278},
		{';',278},
		{'\\',278},
		{'i',278},
		{'j',290},
		{'l',278},
		{'t',278},
		{' ',250},
		{',',250},
		{'.',250},
		{'\'',180},
		{'@', 890},
		{'\0', 0}
};

bool built_character_map = false;
static int SUPPORTED_AMF_CHARACTERS_MAP[256] = {0};

void build_character_map()
{
	for (int ii = 0; SUPPORTED_AMF_CHARACTERS[ii].character != '\0' ; ++ii) {
		const AmfCharacterDefinition at = SUPPORTED_AMF_CHARACTERS[ii];
		SUPPORTED_AMF_CHARACTERS_MAP[(int)at.character] = ii+1;
	}

	built_character_map = true;
}

double amf_text_width_calc(char cc, double font_size)
{
	if (!built_character_map)
		build_character_map();

	int ii = SUPPORTED_AMF_CHARACTERS_MAP[(int)cc];

	if (ii != 0) {
		double wx = SUPPORTED_AMF_CHARACTERS[ii-1].wx;
		wx = wx < 320 ? 320 : wx;
		double width = wx/1000.0 * font_size *1.05;
		return width;
	}
	return 0.5*font_size;
}
