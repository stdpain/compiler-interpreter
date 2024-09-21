#include "logging/interpreter_logger.hpp"
#include "parser/context.h"

void mserror(MSParseContext*, char const* v) {
    PAIN_LOG(v);
}