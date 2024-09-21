#pragma once
#include <memory>
#include <string_view>

#include "ast/ast_builder.h"
#include "parser/context.h"

namespace ms {
class Parser {
public:
    Parser();
    ~Parser();
    int parse(const std::string_view code);

private:
    std::unique_ptr<ASTBuilder> _ast_builder;
    MSParseContext parse_context;
};
} // namespace ms