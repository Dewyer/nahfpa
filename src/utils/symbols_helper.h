//
// Created by barna on 14/11/2020.
//

#ifndef NAHFPA_SYMBOLS_HELPER_H
#define NAHFPA_SYMBOLS_HELPER_H

#define SUPPORTED_SYMBOL_COUNT 39

typedef struct SymbolDefinition {
	char *command;
} SymbolDefinition;

extern SymbolDefinition SUPPORTED_SYMBOLS[SUPPORTED_SYMBOL_COUNT];

#endif //NAHFPA_SYMBOLS_HELPER_H
