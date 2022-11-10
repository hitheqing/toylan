#pragma once
#include <string>
#include <vector>


namespace token
{
    /**
     *  其余的直接使用ascii值
     */
    enum tag
    {
        tok_eof = -1,
        tok_ident = -2,
        tok_number= -3,

        lp = '(',   // 40 parentheses
        rp = ')',   // 41
        ls = '[',   // 91 square brackets
        rs = ']',   // 93
        lc = '{',   // 123 curly brackets
        rc = '}',   // 125
        assign = '=',   // 61
        lt = '<',   // 60
        gt = '>',   // 62
        add = '+', // 43
        sub = '-', // 45
        mul = '*', // 42
        div = '/', // 47
        dot = '.', // 46
        colon = ':', // 58
        comma = ',', // 44
    };

    class Token
    {
    public:
        virtual ~Token() = default;
        int tag;
        std::string meta;

        Token(int tag, std::string value)
            : tag(tag),
              meta(std::move(value))
        {
        }
    };

    class NumToken : public Token
    {
        
    public:
        NumToken(double value)
            : Token(tok_number, ""),
              value(value)
        {
        }
        double value;
    };

    class IdentToken : public Token
    {
    public:
        std::string name;

        IdentToken(const std::string& name)
            : Token(tok_ident, name),
              name(name)
        {
        }
    };
} //namespace token`

namespace ast
{
    class ExprAST
    {
    public:
        virtual ~ExprAST() = default;
        virtual double evaluate() {return 0.0;}
    };


    /**
     *  E*
     *  E*[,E]*,?
     */
    class ExprListExprAST : public ExprAST
    {
        std::vector<ExprAST*> exprs;
    public:
        explicit ExprListExprAST(std::vector<ExprAST*> exprs)
            : exprs(std::move(exprs))
        {
        }
    };


    /**
     *   aaa(exprlist)
     */
    class CallExprAST : public ExprAST
    {
        std::string name;
        std::vector<ExprAST*> args;
    public:
        CallExprAST(std::string name, std::vector<ExprAST*> args)
            : name(std::move(name)),
              args(std::move(args))
        {
        }
    };


    /**
     *  E op E
     */
    class BinOpExprAST : public ExprAST
    {
        char op;
        ExprAST* LHS;
        ExprAST* RHS;
    public:
        BinOpExprAST(char op, ExprAST* lhs, ExprAST* rhs)
            : op(op),
              LHS(lhs),
              RHS(rhs)
        {
        }
        double evaluate() override;
    };

    inline double BinOpExprAST::evaluate()
    {
        switch (op)
        {
        case '+' :return LHS->evaluate() + RHS->evaluate();
        case '-' :return LHS->evaluate() - RHS->evaluate();
        case '*' :return LHS->evaluate() * RHS->evaluate();
        case '/' :return LHS->evaluate() / RHS->evaluate();
            default: return 0.0;
        }
    }


    /**
     *  number   "1.0"
     */
    class NumExprAST : public ExprAST
    {
        double value;
    public:
        explicit NumExprAST(double value)
            : value(value)
        {
        }
        double evaluate() override
        {
            return value;
        }
    };


    /**
     *  identifier  "book"
     */
    class IdentExprAST : public ExprAST
    {
        std::string name;
    public:
        explicit IdentExprAST(std::string name)
            : name(std::move(name))
        {
        }
        double replaced_value = 1.0;
        double evaluate() override
        {
            return replaced_value;
        }
    };
} // namespace ast
