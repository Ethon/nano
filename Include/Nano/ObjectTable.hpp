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

#ifndef HEADER_UUID_9A603D07CC9345FF860A3577B5C4608C
#define HEADER_UUID_9A603D07CC9345FF860A3577B5C4608C

// Nano:
#include <Nano/Util/StringHashtable.hpp>
#include <Nano/Object/Object.hpp>
#include <Nano/Error.hpp>

namespace nano
{
    class UnknownObjectError : public EvaluationError
    {
    private:
        std::string _objectName;
        
    public:
        UnknownObjectError(std::string objectName, SourcePos pos = SourcePos());
        
        std::string const& objectName() const;
    };
    
    class LockedObjectError : public EvaluationError
    {
    private:
        std::string _objectName;
        
    public:
        LockedObjectError(std::string objectName, SourcePos pos = SourcePos());
        
        std::string const& objectName() const;
    };
    
    class ObjectTable
    {
    private:
        struct Entry
        {
            object::Object obj;
            bool locked;
        };
        typedef util::StringHashtable<char, Entry, util::ArenaAllocator> TableType;
        TableType _table;
        ObjectTable* _parent;
        
    public:
        explicit ObjectTable(ObjectTable* parent = nullptr);
        
        object::Object& get(std::string const& name);
        object::Object& set(std::string const& name, object::Object newValue, bool lock = false);
        bool has(std::string const& name) const;
        
        bool isLocked(std::string const& name);
        void lock(std::string const& name, bool newValue);
    };
}

#endif // HEADER_UUID_9A603D07CC9345FF860A3577B5C4608C