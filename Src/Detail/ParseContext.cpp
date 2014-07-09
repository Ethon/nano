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

// C++ Standard Library:
#include <iostream>
#include <stdexcept>
#include <cassert>

// Nano:
#include <Nano/Detail/ParseContext.hpp>

using namespace nano;
using namespace nano::ast;
using namespace nano::detail;

namespace
{
    void onLexerErrorDefault(SourcePos const& pos, char const* s)
    {
        std::cerr << "[Lexer error] (" << pos.line << ',' << pos.col <<
            ") Unrecognized character: " << s << '\n';
    }
    
    void onParserErrorDefault(SourcePos const& pos, char const* s)
    {
        std::cerr << "[Parser error] (" << pos.line << ',' << pos.col <<
            ") Bison error: " << s << '\n';
    }
}

////
// LexerStorage implementation
////

std::string const* LexerStorage::storeIdentifier(std::string identifier)
{
    auto iter = _identifiers.emplace(std::move(identifier)).first;
    return &(*iter);
}

////
// ParseContext implementation
////

ParseContext::ParseContext(char const* string, StringInput)
    :   _inputFile(nullptr, [](std::FILE*){ }),
        _lexer(createStringLexer(string, this), &destroyLexer),
        _onLexerError(&::onLexerErrorDefault),
        _onParserError(&::onParserErrorDefault),
        _storage(),
        _stack()
{ }

ParseContext::ParseContext(char const* fileName, FileInput)
    :   _inputFile(std::fopen(fileName, "r"), [](std::FILE* f) { std::fclose(f); }),
        _lexer(createFileLexer(_inputFile.get(), this), &destroyLexer),
        _onLexerError(&::onLexerErrorDefault),
        _onParserError(&::onParserErrorDefault),
        _storage(),
        _stack()
{
    if(!_inputFile)
        throw std::runtime_error("input file is null");
}

ParseContext::ParseContext(std::FILE* file, bool close)
    :   _inputFile(file, close ? [](std::FILE* f){ std::fclose(f); } : [](std::FILE*){ }),
        _lexer(createFileLexer(_inputFile.get(), this), &destroyLexer),
        _onLexerError(&::onLexerErrorDefault),
        _onParserError(&::onParserErrorDefault),
        _storage(),
        _stack()
{
    if(!_inputFile)
        throw std::runtime_error("input file is null");
}

Node::PtrT ParseContext::parseOne()
{
    assert(_stack.empty());
    parseImpl(this);
    if(_stack.empty())
        return Node::PtrT();
    assert(_stack.size() == 1);
    return pop();
}

void* ParseContext::lexer()
{
    return _lexer.get();
}
            
LexerStorage& ParseContext::storage()
{
    return _storage;
}

void ParseContext::triggerLexerError(SourcePos const& pos, char const* s)
{
    _onLexerError(pos, s);
}

void ParseContext::push(Node::PtrT node)
{
    _stack.emplace(std::move(node));
}

Node::PtrT ParseContext::pop()
{
    Node::PtrT top = std::move(_stack.top());
    _stack.pop();
    return top;
}

Node* ParseContext::top()
{
    return _stack.top().get();
}

void ParseContext::triggerParserError(SourcePos const& pos, char const* s)
{
    _onParserError(pos, s);
}