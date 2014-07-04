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

#ifndef HEADER_UUID_3B6C96A6E8304F83B0F54B5DEDF64FCF
#define HEADER_UUID_3B6C96A6E8304F83B0F54B5DEDF64FCF

// Nano:
#include <Nano/Util/StringHashtable.hpp>
#include <Nano/Object/IntClass.hpp>
#include <Nano/Object/FloatClass.hpp>
#include <Nano/Object/NativeFunctionClass.hpp>
#include <Nano/ObjectTable.hpp>

namespace nano
{
    class GlobalContext
    {
    private:
        object::ClassId _classIdCounter;
        object::IntClass _intClass;
        object::FloatClass _floatClass;
        object::NativeFunctionClass _nativeFunctionClass;
        ObjectTable _globalObjects;
        
    public:
        GlobalContext();
        
        object::ClassId allocateClassId();
        
        // Get classes for builtin types:
        object::IntClass const* getIntClass() const;
        object::FloatClass const* getFloatClass() const;
        object::NativeFunctionClass const* getNativeFunctionClass() const;
        
        ObjectTable& globalObjects();
    };
}

#endif // HEADER_UUID_3B6C96A6E8304F83B0F54B5DEDF64FCF