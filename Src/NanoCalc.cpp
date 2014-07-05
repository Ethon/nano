// C++ Standard Library:
#include <cassert>
#include <iostream>
#include <cstdio>

// Readline:
#include <readline/readline.h>
#include <readline/history.h>

// Nano:
#include <Nano/Detail/ParseContext.hpp>
#include <Nano/GlobalContext.hpp>
#include <Nano/Interpreter.hpp>
            
namespace builtin
{
    nano::object::Object acc(nano::object::Object& callee, nano::object::Object* args, std::size_t count)
    {
        nano::object::Object sum = callee.class_()->globalContext()->getIntClass()->new_(0);
        for(std::size_t i = 0; i < count; ++i)
            sum = sum + *(args  + i);
        return sum;
    }
}

int eval(nano::detail::ParseContext& pctx, nano::Interpreter& interpreter)
{
    int count = 0;
    while(nano::ast::Node::PtrT ast = pctx.parseOne())
    {
        std::cout << interpreter.evaluateExpression(ast.get()).prettyString() << std::endl;
        ++count;
    }
    return count;
}

int main(int argc, char** argv)
{
    nano::GlobalContext gctx;
    nano::Interpreter interpreter((gctx));
    gctx.globalObjects().set("acc", gctx.getNativeFunctionClass()->new_("acc", builtin::acc));
    
    if(argc == 2)
    {
        nano::detail::ParseContext pctx(argv[1], nano::detail::ParseContext::FileInput());
        eval(pctx, interpreter);
    }
    else
    {
        for(;;)
        {
            std::unique_ptr<char, void(*)(void*)> line(readline("> "), &std::free); 
            if(!line)
            {
                std::cout << std::endl;
                break;
            }
            add_history(line.get());
            nano::detail::ParseContext pctx(line.get(), nano::detail::ParseContext::StringInput());
            eval(pctx, interpreter);
        }
    }
}