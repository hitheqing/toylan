#include "ast.h"
#include "parser.h"
#include <map>

double ast::IdentExprAST::evaluate(parser& parser)
{
    if (!replaced_value)
    {
        if (parser.ident_map.find(name) != parser.ident_map.end())
        {
            std::string key = name;
            double v =  parser.ident_map.at(key);
            replaced_value = &(parser.ident_map.find(name)->second);
        }
        else
        {
            printf("请输入标识符'%s'的代入值:", name.c_str());
            double d;
            std::cin >>d;
            
            auto v = parser.ident_map.insert_or_assign(this->name,d);
            replaced_value = &v.first->second;
        }
    }
    return *replaced_value;
}

double ast::BinOpExprAST::evaluate(parser& parser)
{
    switch (op)
    {
    case '+' :return LHS->evaluate(parser) + RHS->evaluate(parser);
    case '-' :return LHS->evaluate(parser) - RHS->evaluate(parser);
    case '*' :return LHS->evaluate(parser) * RHS->evaluate(parser);
    case '/' :return LHS->evaluate(parser) / RHS->evaluate(parser);
    default: return 0.0;
    }
}