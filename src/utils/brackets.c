//
// Created by barna on 2020. 11. 27..
//

#include <assert.h>
#include "brackets.h"

char *get_closing_bracket_for_bracket(const DString *bracket) {
	if (DString_eq_CString(bracket, "{"))
		return "}";
	if (DString_eq_CString(bracket, "("))
		return ")";
	if (DString_eq_CString(bracket, "["))
		return "]";
	if (DString_eq_CString(bracket, "\\{"))
		return "\\}";

	assert("Invalid bracket type used!");
	return NULL;
}
