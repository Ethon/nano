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

#ifndef HEADER_UUID_3B0ACE3EA788443B8E5DEDA9EB29E54D
#define HEADER_UUID_3B0ACE3EA788443B8E5DEDA9EB29E54D

// C++ Standard Library:
#include <stdexcept>

// Acid:
#include <Nano/Common/SourcePos.hpp>

namespace nano
{
    class BaseError : public std::runtime_error
    {
    private:
        SourcePos _pos;
        
    public:
        inline BaseError(SourcePos pos, std::string const& what)
            : std::runtime_error(what), _pos(pos)
        { }
        
        inline SourcePos pos() const
        {
            return _pos;
        }
    };
    
    class EvaluationError : public BaseError
    {
    public:
        inline EvaluationError(SourcePos pos, std::string const& what)
            : BaseError(pos, what)
        { }
    };
}

#endif // HEADER_UUID_3B0ACE3EA788443B8E5DEDA9EB29E54D
