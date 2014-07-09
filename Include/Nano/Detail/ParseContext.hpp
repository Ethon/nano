// Copyright (c) <2014> <Florian Erler>
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

#ifndef HEADER_UUID_6F31B55BD9B8468D829BC8D16F553BEB
#define HEADER_UUID_6F31B55BD9B8468D829BC8D16F553BEB

// C++ Standard Library:
#include <cstdio>
#include <memory>
#include <functional>
#include <string>
#include <unordered_set>
#include <stack>

// Nano:
#include <Nano/Ast/Node.hpp>

namespace nano
{
    namespace detail
    {
        class LexerStorage
        {
        private:
            std::unordered_set<std::string> _identifiers;
            
        public:
            std::string const* storeIdentifier(std::string identifier);
        };
        
        class ParseContext
        {
        public:
            typedef std::function<void (SourcePos const&, char const*)> ErrorCallbackT;
            
            struct StringInput { };
            struct FileInput { };
            
        private:
            std::unique_ptr<std::FILE, void(*)(std::FILE*)> _inputFile;
            std::unique_ptr<void, void(*)(void*)> _lexer;
            ErrorCallbackT _onLexerError;
            ErrorCallbackT _onParserError;
            LexerStorage _storage;
            std::stack<ast::Node::PtrT> _stack;
            
            static void* createFileLexer(std::FILE* file, ParseContext* context);
            static void* createStringLexer(char const* string, ParseContext* context);
            static void destroyLexer(void* lexer);
            static int parseImpl(ParseContext* context);

        public:
            ParseContext() = default;
            ParseContext(char const* string, StringInput);
            ParseContext(char const* fileName, FileInput);
            ParseContext(std::FILE* file, bool close);
            
            // Public interface:
            ast::Node::PtrT parseOne();
            
            // Interface used by the Lexer:
            void* lexer();
            LexerStorage& storage();
            void triggerLexerError(SourcePos const& pos, char const* s);
            
            // Interface used by the Parser.
            void push(ast::Node::PtrT node);
            ast::Node::PtrT pop();
            ast::Node* top();
            void triggerParserError(SourcePos const& pos, char const* s);
            
            template<typename T, typename... Args>
            void push(Args&&... args)
            {
                push(ast::Node::make<T>(std::forward<Args>(args)...));
            }
        };
    }
}

#endif // HEADER_UUID_6F31B55BD9B8468D829BC8D16F553BEB