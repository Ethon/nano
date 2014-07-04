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

// Nano:
#include <Nano/GlobalContext.hpp>

////
// GlobalContext implementation
////

nano::GlobalContext::GlobalContext()
    : _classIdCounter(1),
      _intClass(this, allocateClassId()),
      _floatClass(this, allocateClassId()),
      _nativeFunctionClass(this, allocateClassId()),
      _globalObjects()
{ }
        
nano::object::ClassId nano::GlobalContext::allocateClassId()
{
    return _classIdCounter++; 
}

nano::object::IntClass const* nano::GlobalContext::getIntClass() const
{
    return &_intClass;
}

nano::object::FloatClass const* nano::GlobalContext::getFloatClass() const
{
    return &_floatClass;
}

nano::object::NativeFunctionClass const* nano::GlobalContext::getNativeFunctionClass() const
{
    return &_nativeFunctionClass;
}

nano::ObjectTable& nano::GlobalContext::globalObjects()
{
    return _globalObjects;
}