# where to put generated libraries
# set(LIBRARY_OUTPUT_PATH "${BUILD_DIR}/src/gen")

# where to put generated binaries
# set(EXECUTABLE_OUTPUT_PATH "${BUILD_DIR}/src/gen")

message(STATUS ${GENSRC_DIR})
add_library(Gen STATIC
    ${GENSRC_DIR}/gen/ms_lex.l.cpp
    ${GENSRC_DIR}/gen/ms_yacc.y.cpp
    ./error.cpp
)

add_custom_command(
    OUTPUT ${GENSRC_DIR}/gen/ms_yacc.y.cpp  ${GENSRC_DIR}/gen/ms_yacc.y.hpp
    COMMAND mkdir -p ${GENSRC_DIR}/gen
    COMMAND bison --defines --output=${GENSRC_DIR}/gen/ms_yacc.y.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ms_yacc.y
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/ms_yacc.y)
set_source_files_properties(${GENSRC_DIR}/gen/ms_yacc.y.cpp PROPERTIES GENERATED TRUE)

add_custom_command(
    OUTPUT ${GENSRC_DIR}/gen/ms_lex.l.cpp ${GENSRC_DIR}/gen/ms_lex.l.h
    COMMAND mkdir -p ${GENSRC_DIR}/gen
    COMMAND flex --header-file=${GENSRC_DIR}/gen/ms_lex.l.h --outfile=${GENSRC_DIR}/gen/ms_lex.l.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ms_lex.l
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/ms_lex.l)
set_source_files_properties(${GENSRC_DIR}/gen/ms_lex.l.cpp PROPERTIES GENERATED TRUE)


