#include "parser.h"

#include <utility>

ExprAST* LogError(const char* str)
{
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

Token* parser::get_next_token()
{
    cur = _get_token();
    return cur;
}

Token* parser::_get_token()
{
    for (; *ptr_cur != '\0'; ptr_cur++)
    {
        if (isspace(*ptr_cur))
        {
            continue;
        }

        // digit := [0-9.]+
        if (isdigit(*ptr_cur))
        {
            std::string s;
            do
            {
                s += *ptr_cur;
                ptr_cur++;
            }
            while (isdigit(*ptr_cur));
            return (Token*)(new NumToken(strtod(s.c_str(), nullptr)));
        }

        // ident := [a-zA-Z]+
        if (isalpha(*ptr_cur))
        {
            std::string s;
            do
            {
                s += *ptr_cur;
                ptr_cur++;
            }
            while (isalpha(*ptr_cur));
            return (Token*)(new IdentToken(s));
        }

        std::string s;
        s += *ptr_cur;
        return new Token(*ptr_cur++, s);
    }
    return new Token(tok_eof, "");
}

void parser::reset()
{
    cur = nullptr;
}

/**
 *  E   ::=  E [op E']*
 *      ::=  id | num | (E')
 *
 *  E ::= primary binoprhs*
 */
ExprAST* parser::parse_expr()
{
    auto LHS = parse_primary_expr();
    if (!LHS)
    {
        return nullptr;
    }

    return parse_binop_rhs(0, LHS);
}

/**
 *  primary  
 *      ::=  id | num | (E)
 */
ExprAST* parser::parse_primary_expr()
{
    switch (cur->tag)
    {
    case tag::tok_ident:
        return parse_ident_expr();
    case tag::tok_number:
        return parse_num_expr();
    case tag::lp:
        return parse_paren_expr();
    }
    return nullptr;
}

int parser::get_priority(int tag)
{
    switch (tag)
    {
    case '+': return 20;
    case '-': return 20;
    case '*': return 40;
    case '/': return 40;
    default: return -1;
    }
}

// binoprhs ::= (op primary)*
ExprAST* parser::parse_binop_rhs(int priority, ExprAST* lhs)
{
    while (true)
    {
        int opPriority = get_priority(cur->tag);
        if (opPriority < priority)
        {
            return lhs;
        }
        char left_op = (char)cur->tag;
        get_next_token(); // eat op

        auto RHS = parse_primary_expr();
        if (!RHS)
        {
            return nullptr;
        }

        char right_op = (char)cur->tag;
        int right_op_priority = get_priority(right_op);
        if (right_op_priority > opPriority) // 右边优先， 将rhs作为lhs递归调用，结果作为新的rhs替换
        {
            RHS = parse_binop_rhs(right_op_priority, RHS);
            if (!RHS)
            {
                return nullptr;
            }
        }

        // 合并 表达式，作为新的 lhs
        lhs = new BinOpExprAST(left_op, lhs, RHS);
    }
}

// identexpr    ::= ident
//              ::= ident '(' expr*  ')'   // 函数调用
ExprAST* parser::parse_ident_expr()
{
    auto token = dynamic_cast<IdentToken*>(cur);
    get_next_token(); // eat token
    if (!token)
    {
        return nullptr;
    }
    if (cur->tag != lp) // simple variable
    {
        auto V = new IdentExprAST(token->name);
        if (ident_map.find(token->name) != ident_map.end())
        {
            V->replaced_value = & ident_map[token->name];
        }
        return V;
    }

    // call
    get_next_token(); // eat (
    std::vector<ExprAST*> args;
    if (cur->tag != rp) // parse args
    {
        while (true)
        {
            auto E = parse_expr();
            if (E)
            {
                args.push_back(E);
            }
            else
            {
                return LogError("expect expr args");
            }

            if (cur->tag == rp)
            {
                break;
            }
            if (cur->tag != ',')
            {
                return LogError("expect ',' in args list");
            }
            get_next_token(); // eat ,
        }
    }

    if (cur->tag != rp) // parse args
        return LogError("expect ')', in args list");

    auto V = new CallExprAST(token->name, args);
    return V;
}

// numexpr ::= num
ExprAST* parser::parse_num_expr()
{
    auto token = dynamic_cast<NumToken*>(cur);
    get_next_token(); // eat token
    if (!token)
    {
        return nullptr;
    }

    auto V = new NumExprAST(token->value);
    return V;
}


// paren ::=  '(' E ')'  
ExprAST* parser::parse_paren_expr()
{
    get_next_token(); // eat (
    auto V = parse_expr();
    if (!V)
    {
        return nullptr;
    }
    if (cur->tag != tag::rp)
    {
        return LogError("expect )");
    }
    get_next_token(); // eat )
    return V;
}



ExprAST* parser::parse_text(const char* text)
{
    ptr_cur = text;
    reset();

    get_next_token();
    return parse_expr();
}

void parser::set_ident_map( std::map<std::string, double> map)
{
    ident_map = std::move(map);
}
