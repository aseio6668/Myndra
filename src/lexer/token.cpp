#include "token.h"

namespace myndra {

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::NIL: return "NIL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::LET: return "LET";
        case TokenType::FN: return "FN";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::RETURN: return "RETURN";
        case TokenType::IMPORT: return "IMPORT";
        case TokenType::EXPORT: return "EXPORT";
        case TokenType::WITH: return "WITH";
        case TokenType::CAPABILITIES: return "CAPABILITIES";
        case TokenType::CAPSULE: return "CAPSULE";
        case TokenType::DSL: return "DSL";
        case TokenType::FALLBACK: return "FALLBACK";
        case TokenType::RETRY: return "RETRY";
        case TokenType::CONTEXT: return "CONTEXT";
        case TokenType::OVER: return "OVER";
        case TokenType::TAG: return "TAG";
        case TokenType::DID: return "DID";
        case TokenType::EVOLVING: return "EVOLVING";
        case TokenType::AT_SYNC: return "@SYNC";
        case TokenType::AT_ASYNC: return "@ASYNC";
        case TokenType::AT_PARALLEL: return "@PARALLEL";
        case TokenType::AT_REACTIVE: return "@REACTIVE";
        case TokenType::AT_TEMPORAL: return "@TEMPORAL";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TokenType::ARROW: return "ARROW";
        case TokenType::FAT_ARROW: return "FAT_ARROW";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::COLON: return "COLON";
        case TokenType::DOUBLE_COLON: return "DOUBLE_COLON";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_ANGLE: return "LEFT_ANGLE";
        case TokenType::RIGHT_ANGLE: return "RIGHT_ANGLE";
        case TokenType::HASH: return "HASH";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::IF_CONTEXT: return "IF_CONTEXT";
        case TokenType::OBSERVABLE: return "OBSERVABLE";
        case TokenType::SUBSCRIBE: return "SUBSCRIBE";
        case TokenType::EMIT: return "EMIT";
        case TokenType::TRANSITION: return "TRANSITION";
        case TokenType::TIMELINE: return "TIMELINE";
        case TokenType::VERIFY: return "VERIFY";
        case TokenType::PROOF: return "PROOF";
        case TokenType::HAS_PROOF: return "HAS_PROOF";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

} // namespace myndra
