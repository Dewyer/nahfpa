//
// Created by barna on 14/11/2020.
//

#include <stddef.h>
#include <ctype.h>
#include "symbols_helper.h"
#include "geomerty.h"

const SymbolDefinition SUPPORTED_SYMBOLS[] = {
		{"\\int",   "∫", NULL, {12, 38}, {0,0}},
		{"\\alpha", "α", "Α", {19, 35},{25,35}},
		{"\\beta",  "β", "Β", {19, 40},{23,35}},
		{"\\gamma",  "γ", "Γ", {17, 40},{20,35}},
		{"\\epsilon",  "ε", "E", {17, 35},{25,35}},
		{"\\zeta",  "ζ", "Ζ", {17, 40},{22,35}},
		{"\\eta",  "ή", "H", {17, 40},{22,35}},
		{"\\theta",  "θ", "Θ", {17, 40},{23,35}},
		{"\\iota",  "ί", "I", {13, 35},{13,35}},
		{"\\kappa",  "θ", "Θ", {17, 35},{23,35}},
		{"\\lambda",  "λ", "Λ", {17, 35},{23,35}},
		{"\\mu",  "μ", "M", {17, 40},{28,35}},
		{"\\nu",  "ν", "V", {17, 35},{24,35}},
		{"\\xi",  "ξ", "Ξ", {17, 40},{23,35}},
		{"\\omicron",  "ο", "O", {17, 35},{23,35}},
		{"\\pi",  "π", "Π", {17, 35},{23,35}},
		{"\\rho",  "ρ", "P", {17, 40},{20,35}},
		{"\\sigma",  "σ", "Σ", {17, 35},{20,35}},
		{"\\tau",  "τ", "T", {17, 35},{22,35}},
		{"\\upsilon",  "υ", "Y", {17, 35},{22,35}},
		{"\\phi",  "φ", "Φ", {17, 40},{22,35}},
		{"\\chi",  "x", "X", {17, 35},{22,35}},
		{"\\psi",  "ψ", "Ψ", {17, 40},{23,35}},
		{"\\omega",  "ω", "Ω", {22, 35},{25,35}},
		{"\\leq",  "≤", NULL, {19, 35},{0,0}},
		{"\\geq",  "≥", NULL, {19, 35},{0,0}},
		{"\\neq",  "≠", NULL, {19, 35},{0,0}},
		{"\\sim",  "~", NULL, {19, 35},{0,0}},
		{"\\to",  "→", NULL, {33, 35},{0,0}},
		{"\\gets",  "←", NULL, {33, 35},{0,0}},
		{"\\cdot",  "⋅", NULL, {11, 35},{0,0}},
		{"\\dots",  "…", NULL, {33, 35},{0,0}},
		{"\\exists",  "∃", NULL, {19, 35},{0,0}},
		{"\\forall",  "∀", NULL, {19, 35},{0,0}},
		{"\\approx",  "≈", NULL, {19, 35},{0,0}},
		{"\\subset",  "⊂", NULL, {22, 35},{0,0}},
		{"\\in",  "∊", NULL, {19, 35},{0,0}},
		{"\\cap",  "⋂", NULL, {23, 35},{0,0}},
		{"\\inf",  "∞", NULL, {24, 35},{0,0}},

		{NULL, NULL, NULL, {0,0}, {0,0}}
};

SymbolDefinitionFindResults SymbolDefinition_get_supported_results(DString *command)
{
	SymbolDefinitionFindResults results = {false, NULL, false};
	DString *command_copy = DString_clone(command);
	char *cmd_str_lower = DString_to_CString(command_copy);
	if (DString_len(command) > 1)
		cmd_str_lower[1] = tolower(cmd_str_lower[1]);

	DString *cmd_lower = DString_from_CString(cmd_str_lower);

	for (int ii = 0; SUPPORTED_SYMBOLS[ii].command; ++ii) {
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
	DString_free(command_copy);
	return results;
}
