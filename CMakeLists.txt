cmake_minimum_required(VERSION 3.30)

project(cca)
enable_testing()

add_library(cca_lexer
    token.c         token.h
    lexer.c         lexer.h
    util.c          util.h
)

add_library(cca_parser_pratt
    parser_pratt.c  parser_pratt.h
    token.c         token.h
    lexer.c         lexer.h
    util.c          util.h
)

add_executable(cca 
    cca.c
)

target_link_libraries(cca cca_lexer)
target_link_libraries(cca cca_parser_pratt)


# cca testing
add_executable(cca_test
    test.c
)

target_link_libraries(cca_test cca_lexer)
target_link_libraries(cca cca_parser_pratt)

add_test(cca_test
    NAME cca_test
    COMMAND cca_test
)
