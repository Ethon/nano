#ifndef HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF
#define HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF

// Nano:
#include <Nano/GlobalContext.hpp>
#include <Nano/Ast/Node.hpp>

namespace nano
{
    class Interpreter
    {
    private:
        GlobalContext& _ctx;
        
    public:
        explicit Interpreter(GlobalContext& ctx);
        
        object::Object evaluateExpression(ast::Node* expressionAst);
    };
}

#endif // HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF