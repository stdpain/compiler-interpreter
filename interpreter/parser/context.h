#pragma once

#include "ast/ast_builder.h"

namespace ms {
class ASTBuilder;
}

struct MSParseContext {
    void* scanner;
    ms::ASTBuilder* ast_builder;
};