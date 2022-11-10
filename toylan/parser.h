#pragma once
#include <map>

#include "ast.h"
using namespace token;
using namespace ast;

static std::string temp_str;

class parser
{
    Token* get_next_token();
    Token* _get_token();
    Token* cur = nullptr;
    const char* ptr_cur = nullptr;
    std::map<std::string, double> ident_map;
    void reset();

    static int get_priority(int tag);
    ExprAST* parse_ident_expr();
    ExprAST* parse_num_expr();
    ExprAST* parse_paren_expr();
    ExprAST* parse_primary_expr();
    ExprAST* parse_binop_rhs(int priority, ExprAST* lhs);
    ExprAST* parse_expr();
public:
    ExprAST* parse_text(const char* text);

    // for calc
    void set_ident_map(const std::map<std::string, double>& map);
};
