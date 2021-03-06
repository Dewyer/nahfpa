cmake_minimum_required(VERSION 3.17)
project(nahfpa C)

set(CMAKE_C_STANDARD 99)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

SET(BASEPATH "${CMAKE_SOURCE_DIR}/src")
INCLUDE_DIRECTORIES("${BASEPATH}")

add_executable(nahfpa
        src/main.c
        src/data_structures/List.c
        src/data_structures/List.h
        src/expression_parser/ExpParser.c
        src/expression_parser/ExpParser.h
        src/data_structures/DString.c
        src/data_structures/DString.h
        src/expression_tokenizer/ExpTokenizer.c
        src/expression_tokenizer/ExpTokenizer.h
        src/expression_parser/ExpNode.c
        src/expression_parser/ExpNode.h
        src/data_structures/common.h
        src/logger/logger.c
        src/logger/logger.h
        src/nahfpa_cli/NahfpaCli.c
        src/nahfpa_cli/NahfpaCli.h
        src/utils/cassert.c
        src/utils/cassert.h
        src/utils/help_txt.h
        src/utils/help_txt.c
        src/utils/help_txt.h
        src/expression_parser/TokenSlice.c
        src/expression_parser/TokenSlice.h
        src/utils/symbols_helper.c
        src/utils/symbols_helper.h
        src/svg_factory/SvgFactory.c
        src/svg_factory/SvgFactory.h
        src/svg_file/SvgFile.c
        src/svg_file/SvgFile.h
        src/svg_factory/BoxNode.c
        src/svg_factory/BoxNode.h
        src/utils/geomerty.c
        src/utils/geomerty.h
        src/utils/rendering_constants.c
        src/utils/rendering_constants.h
        src/svg_factory/BoxModelBuilder.c
        src/svg_factory/BoxModelBuilder.h
        src/svg_factory/AmfTextWidth.h
        src/svg_factory/AmfTextWidth.c src/utils/brackets.c src/utils/brackets.h)