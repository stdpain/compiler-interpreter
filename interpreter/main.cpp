#include <cstdio>
#include <cstdlib>

#include "logging/interpreter_logger.hpp"
#include "parser/parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        LOG(WARNING) << "usage: ./interpreter 'a=1;'";
        return 1;
    }
    ms::Parser parser;
    LOG(INFO) << "input code" << (argv[1]);
    parser.parse(argv[1]);
    return 0;
}