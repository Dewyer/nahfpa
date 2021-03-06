#include <string.h>
#include <logger/logger.h>
#include <stdbool.h>
#include <utils/cassert.h>
#include <utils/help_txt.h>
#include <svg_factory/SvgFactory.h>
#include "NahfpaCli.h"
#include "data_structures/DString.h"
#include "expression_parser/ExpParser.h"
#include "debugmalloc.h"

typedef enum CliMode {
	HelpMode, CompileMode
} CliMode;

struct NahfpaCli {
	int arg_count;
	char **args;

	char *input_file_path;
	char *out_file_path;
	char *log_file_path;
	char *need_color;
	LogLevel min_log_level;

	Logger *pre_logger;
	Logger *logger;
};

NahfpaCli *NahfpaCli_new(int argc, char *argv[]) {
	NahfpaCli *self = (NahfpaCli *) malloc(sizeof(*self));
	self->arg_count = argc;
	self->args = argv;

	self->min_log_level = LogInfo;
	self->input_file_path = NULL;
	self->out_file_path = NULL;
	self->log_file_path = NULL;
	self->need_color = NULL;

	self->logger = NULL;
	self->pre_logger = Logger_new(LogInfo, NULL, false);

	return self;
}

void NahfpaCli_parse_min_log_level_from_string(NahfpaCli *self, char *log_level_str) {
	const int levels_count = 3;
	char *level_names[][2] = {
			{"i", "info"},
			{"w", "warning"},
			{"e", "error"}
	};

	if (log_level_str == NULL) {
		self->min_log_level = LogInfo;
		return;
	}

	for (int ii = 0; ii < levels_count; ++ii)
		if (strcmp(log_level_str, level_names[ii][0]) == 0 || strcmp(log_level_str, level_names[ii][1]) == 0)
			self->min_log_level = (LogLevel) ii;
}

void NahfpaCli_log_args(const NahfpaCli *self) {
	Logger_log(self->pre_logger, LogInfo, "Supplied / Default CLI arguments:");
	Logger_log(self->pre_logger, LogInfo, "\tInput file: %s", self->input_file_path ? self->input_file_path : "STDIN");
	Logger_log(self->pre_logger, LogInfo, "\tOutput file path: %s", self->out_file_path);
	Logger_log(self->pre_logger, LogInfo, "\tLog file: %s", self->log_file_path ? self->log_file_path : "STDOUT");
	Logger_log(self->pre_logger, LogInfo, "\tMinimum log level: %s", LogLevel_to_string(self->min_log_level));
}

CliMode NahfpaCli_parse_args_get_cli_mode(NahfpaCli *self) {
	Logger_log(self->pre_logger, LogInfo, "Parsing CLI arguments.");

	const int arg_type_count = 6;
	const char *arg_names[][2] = {
			{"-i",  "--input"},
			{"-o",  "--out"},
			{"-l",  "--log-level"},
			{"-lf", "--log-file"},
			{"-c",  "--color"},
			{"-h",  "--help"}
	};
	char *log_level_str = NULL;
	char **arg_ptrs[] = {&self->input_file_path, &self->out_file_path, &log_level_str, &self->log_file_path,
						 &self->need_color};

	for (int arg_i = 1; arg_i < self->arg_count; arg_i++) {
		char *arg = self->args[arg_i];
		bool found_name = false;

		for (int arg_type_i = 0; arg_type_i < arg_type_count; ++arg_type_i)
			if ((strcmp(arg, arg_names[arg_type_i][0]) == 0 || strcmp(arg, arg_names[arg_type_i][1]) == 0)) {
				if (arg_type_i == arg_type_count - 1)
					return HelpMode; // Help flag triggers help mode

				if (*(arg_ptrs[arg_type_i]) == NULL) {
					*(arg_ptrs[arg_type_i]) = self->args[arg_i + 1];
					found_name = true;
					arg_i++; // To skip next arg
				} else {
					Logger_log(self->pre_logger, LogWarning, "Value for argument: %s, already specified", arg);
					found_name = true;
				}
			}

		if (!found_name)
			Logger_log(self->pre_logger, LogWarning, "Invalid command line argument: %s", arg);
	}

	if (self->out_file_path == NULL) // default out path
		self->out_file_path = "./out.svg";

	NahfpaCli_parse_min_log_level_from_string(self, log_level_str);
	NahfpaCli_log_args(self);

	self->logger = Logger_new(self->min_log_level, self->log_file_path,
							  self->need_color ? strcmp(self->need_color, "yes") == 0 : false);

	return CompileMode;
}

DString *NahfpaCli_read_input(NahfpaCli *self) {
	Logger_log(self->logger, LogInfo, "Reading script input from: %s:",
			   self->input_file_path ? self->input_file_path : "STDIN");
	FILE *inp;
	bool should_close_inp = false;
	if (self->input_file_path) {
		inp = fopen(self->input_file_path, "r");
		should_close_inp = true;

		cassert(self->logger, inp != NULL, "Couldn't open input file to read.");
	} else
		inp = stdin;

	DString *full_input_string = DString_new();
	char inp_char;
	while (fscanf(inp, "%c", &inp_char) != EOF && inp_char != '\n') {
		DString_add_char(full_input_string, inp_char);
	}

	if (should_close_inp)
		fclose(inp);

	const size_t read_len = DString_len(full_input_string);
	Logger_log(self->logger, LogInfo, "Read %d characters", read_len);
	cassert(self->logger, read_len > 0, "Input script needs to be longer than 0 characters.");

	return full_input_string;
}

void NahfpaCli_compile(NahfpaCli *self) {
	Logger_log(self->logger, LogInfo, "NAPFHA loaded...");

	DString *latex = NahfpaCli_read_input(self);
	ExpParser *parser = ExpParser_new(self->logger, latex);

	ExpNode *exp_tree_root = ExpParser_parse(parser);

	SvgFactory *svg_factory = SvgFactory_new(self->logger, exp_tree_root, self->out_file_path);
	SvgFactory_create(svg_factory);

	ExpNode_free(exp_tree_root);
	SvgFactory_free(svg_factory);
	ExpParser_free(parser);
}

void NahpfaCli_show_help() {
	printf("%s", HELP_TEXT);
}

void NahfpaCli_execute(NahfpaCli *self) {
	Logger_log(self->pre_logger, LogInfo, "NAPFHA - the MathTex like compiler");
	CliMode running_mode = NahfpaCli_parse_args_get_cli_mode(self);

	switch (running_mode) {
		case HelpMode:
			NahpfaCli_show_help();
			break;
		case CompileMode:
			NahfpaCli_compile(self);
			break;
		default:
			break;
	}
}

void NahfpaCli_free(NahfpaCli *self) {
	if (self->logger)
		Logger_free(self->logger);

	Logger_free(self->pre_logger);
	free(self);
}
