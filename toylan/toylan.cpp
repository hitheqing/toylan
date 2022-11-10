
#include <string>

#include "parser.h"

int main(int argc, char* argv[])
{
    {
        const char* text = "88/2 + 2 * (3 - 4)+8  ";
        parser parser;
        auto v = parser.parse_text(text);
        printf("%s = %f\n",text, v->evaluate());
    }

    {
        const char* text = "(x+y)*(x-y)+1";
        parser parser;
        std::map<std::string,double> value_map  = {
            {"x",5},
            {"y",4},
        };
        parser.set_ident_map(value_map);
        
        auto v = parser.parse_text(text);

        printf("%s = %f\n",text, v->evaluate());
    }
    
    return 0;
}
