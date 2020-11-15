//
// Created by barna on 14/11/2020.
//

#include <stddef.h>
#include <ctype.h>
#include "symbols_helper.h"
#include "geomerty.h"

const SymbolDefinition SUPPORTED_SYMBOLS[SUPPORTED_SYMBOL_COUNT] = {
		{"\\int",   "∫", NULL},
		{"\\alpha", "α", "Α"},
		{"\\beta",  "β", "Β"}/*
		{"\\gamma"},
		{"\\epsilon"},
		{"\\zeta"},
		{"\\eta"},
		{"\\theta"},
		{"\\iota"},
		{"\\kappa"},
		{"\\lambda"},
		{"\\mu"},
		{"\\nu"},
		{"\\xi"},
		{"\\omicron"},
		{"\\pi"},
		{"\\rho"},
		{"\\sigma"},
		{"\\tau"},
		{"\\upsilon"},
		{"\\phi"},
		{"\\chi"},
		{"\\psi"},
		{"\\omega"},
		{"\\le"},
		{"\\ge"},
		{"\\neq"},
		{"\\sim"},
		{"\\to"},
		{"\\gets"},
		{"\\cdot"},
		{"\\dots"},
		{"\\exists"},
		{"\\forall"},
		{"\\approx"},
		{"\\subset"},
		{"\\in"},
		{"\\cap"},
		{"\\inf"}*/
};

SymbolDefinitionFindResults SymbolDefinition_get_supported_results(DString *command)
{
	SymbolDefinitionFindResults results = {false, NULL, false};

	char *cmd_str_lower = DString_to_CString(command);
	if (DString_len(command) > 1)
		cmd_str_lower[1] = tolower(cmd_str_lower[1]);

	DString *cmd_lower = DString_from_CString(cmd_str_lower);

	for (int ii = 0; ii < SUPPORTED_SYMBOL_COUNT; ++ii) {
		const SymbolDefinition *at_definition = &SUPPORTED_SYMBOLS[ii];
		if (DString_eq_CString(command, at_definition->command)) {
			results.found = true;
			results.definition = at_definition;
		} else if (at_definition->uppercase_substitution && DString_eq_CString(cmd_lower, at_definition->command)) {
			results.found = true;
			results.definition = at_definition;
			results.is_uppercase = true;
		}
	}

	DString_free(cmd_lower);
	return results;
}
