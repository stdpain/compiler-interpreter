#include "ast/parser.h"

namespace stdpain
{
void Parser::append_statement(Statement* statement) {
    PAIN_LOG(statement);
    _statement_list.push_back(statement);
}
} // namespace stdpain
