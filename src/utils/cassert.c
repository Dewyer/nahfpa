//
// Created by barna on 03/11/2020.
//

#include <assert.h>
#include "cassert.h"
#include "debugmalloc.h"

void cassert(Logger *log_inst, int exp, char* msg)
{
	if (!exp)
	{
		Logger_log(log_inst, LogError, msg);
		assert(exp);
	}
}
