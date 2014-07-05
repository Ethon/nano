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
#include <cassert>
#include <memory>

// Nano:
#include <Nano/Object/NativeFunctionClass.hpp>
#include <Nano/GlobalContext.hpp>

nano::object::NativeFunctionClass::NativeFunctionClass(GlobalContext* globalContext, ClassId id)
    : Class(globalContext, id, "NativeFunction")
{ }

std::string nano::object::NativeFunctionClass::prettyString(Object const& obj) const
{
    assert(is(obj));
    std::string result = "<NativeFunction '";
    result += obj._data.nfunctionval->name;
    result += "'>";
    return result;
}

nano::object::Object nano::object::NativeFunctionClass::new_(std::string name,
        std::function<Object (Object&, Object*, std::size_t)> function) const
{
    Object result(this);
    std::unique_ptr<Object::NativeFunctionData> data(new Object::NativeFunctionData);
    data->name = std::move(name);
    data->function = std::move(function);
    result._data.nfunctionval = data.release();
    return result;
}

void nano::object::NativeFunctionClass::copyData(Object const& obj, Object& target) const
{
    Class::copyData(obj, target);
    target._data.nfunctionval->ref();
}

void nano::object::NativeFunctionClass::delete_(Object& obj) const
{
    assert(is(obj));
    if(obj._data.nfunctionval->isLastRef())
        delete obj._data.nfunctionval;
    else
        obj._data.nfunctionval->unref();
}

nano::object::Object nano::object::NativeFunctionClass::call(Object& obj,
        Object* args, std::size_t argCount) const
{
    assert(is(obj));
    return obj._data.nfunctionval->function(obj, args, argCount);
}
        
bool nano::object::isNativeFunction(Object const& obj)
{
    return obj.class_()->globalContext()->getNativeFunctionClass()->is(obj);
}