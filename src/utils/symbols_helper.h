//
// Created by barna on 14/11/2020.
//

#ifndef NAHFPA_SYMBOLS_HELPER_H
#define NAHFPA_SYMBOLS_HELPER_H

#include <stdbool.h>
#include <data_structures/DString.h>

#define SUPPORTED_SYMBOL_COUNT 3


typedef struct SymbolDefinition {
	char *command;
	char *substitution;
	char *uppercase_substitution;
} SymbolDefinition;

typedef struct SymbolDefinitionFindResults
{
	bool found;
	const SymbolDefinition *definition;
	bool is_uppercase;
} SymbolDefinitionFindResults;

extern const SymbolDefinition SUPPORTED_SYMBOLS[SUPPORTED_SYMBOL_COUNT];

SymbolDefinitionFindResults SymbolDefinition_get_supported_results(DString *command);

#endif //NAHFPA_SYMBOLS_HELPER_H
