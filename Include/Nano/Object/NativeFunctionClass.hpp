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

#ifndef HEADER_UUID_4299409B858B43D4B14C51D8562E347A
#define HEADER_UUID_4299409B858B43D4B14C51D8562E347A

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class NativeFunctionClass : public Class
        {
        public:
            NativeFunctionClass(GlobalContext* globalContext, ClassId id);
            
            virtual std::string prettyString(Object const& o) const override;
            
            Object new_(std::string name, std::function<Object (Object&, Object*, std::size_t)> function) const;
            virtual void copyData(Object const& obj, Object& target) const;
            virtual void delete_(Object& obj) const;
            
            virtual Object call(Object& obj, Object* args, std::size_t argCount) const override;
        };
        
        bool isNativeFunction(Object const& obj);
    }
}

#endif // HEADER_UUID_4299409B858B43D4B14C51D8562E347A
