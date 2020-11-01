cmake_minimum_required(VERSION 3.17)
project(nahfpa C)

set(CMAKE_C_STANDARD 99)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


add_executable(nahfpa main.c data_structures/List.c data_structures/List.h expression_parser/ExpParser.c expression_parser/ExpParser.h data_structures/DString.c data_structures/DString.h expression_tokenizer/ExpTokenizer.c expression_tokenizer/ExpTokenizer.h expression_parser/ExpNode.c expression_parser/ExpNode.h data_structures/common.h)