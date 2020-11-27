//
// Created by barna on 03/11/2020.
//

#include <assert.h>
#include "cassert.h"
#include "debugmalloc.h"

void cassert(Logger *log_inst, int exp, char *msg, ...) {

	if (!exp) {
		va_list ap;
		va_start(ap, msg);
		Logger_log_varg(log_inst, LogError, msg, ap);

		va_end(ap);
		assert(exp);
	}

}
