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
#include <Nano/ObjectTable.hpp>

////
// UnknownObjectError implementation
////

nano::UnknownObjectError::UnknownObjectError(std::string objectName, SourcePos pos)
    : EvaluationError(pos, "unknown object"), _objectName(std::move(objectName))
{ }
        
std::string const& nano::UnknownObjectError::objectName() const
{
    return _objectName;
}

////
// LockedObjectError implementation
////

nano::LockedObjectError::LockedObjectError(std::string objectName, SourcePos pos)
    : EvaluationError(pos, "locked object"), _objectName(std::move(objectName))
{ }
        
std::string const& nano::LockedObjectError::objectName() const
{
    return _objectName;
}

////
// ObjectTable implementation
////

nano::ObjectTable::ObjectTable(ObjectTable* parent)
    : _table(), _parent(parent)
{ }

nano::object::Object& nano::ObjectTable::get(std::string const& name)
{
    auto result =_table.get(name);
    if(!result)
        throw UnknownObjectError(name);
    return result->obj;
}

nano::object::Object& nano::ObjectTable::set(std::string const& name, object::Object newValue, bool lock)
{
    auto existing = _table.get(name);
    if(existing)
    {
        if(existing->locked)
            throw LockedObjectError(name);
        return (existing->obj = std::move(newValue));
    }
    
    return _table.insert(name, Entry{std::move(newValue), lock}).first->obj;
}

bool nano::ObjectTable::has(std::string const& name) const
{
    return _table.count(name) != 0;
}

bool nano::ObjectTable::isLocked(std::string const& name)
{
    auto entry =_table.get(name);
    return entry ? entry->locked : false;
}

void nano::ObjectTable::lock(std::string const& name, bool newValue)
{
    auto entry =_table.get(name);
    if(!entry)
        throw UnknownObjectError(name);
    entry->locked = newValue;
}