#include "parser/parser.h"

#include <memory>

#include "ast/ast_builder.h"
#include "gen/forward.h"

namespace ms {
Parser::Parser() {
    mslex_init_extra(&parse_context, &parse_context.scanner);
    _ast_builder = std::make_unique<ASTBuilder>();
    parse_context.ast_builder = _ast_builder.get();
}

Parser::~Parser() {
    if (parse_context.scanner != nullptr) {
        mslex_destroy(parse_context.scanner);
    }
}

int Parser::parse(const std::string_view code) {
    ms_scan_bytes(code.data(), code.length(), parse_context.scanner);
    return msparse(&parse_context);
}

} // namespace ms