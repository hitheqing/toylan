
#include <string>

#include "parser.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

int main(int argc, char* argv[])
{
    {
        const char* text = "88/2 + 2 * (3 - 4)+8  ";
        parser parser;
        auto v = parser.parse_text(text);
        printf("evaluate: %s \n", text);
        printf("%s = %f\n", text, v->evaluate(parser));
    }

    {
        const char* text = "(x+y)*(x-y)+1";
        parser parser;
        
        auto value_map = {
            {"x", 5},
            {"yy", 4},
        };
        parser.set_ident_map(value_map);

        auto v = parser.parse_text(text);
        printf("evaluate: %s \n", text);
        printf("%s = %f\n", text, v->evaluate(parser));
    }

    return 0;
}
