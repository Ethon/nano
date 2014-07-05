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

#ifndef HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2
#define HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class FloatClass : public Class
        {
        public:
            FloatClass(GlobalContext* globalContext, ClassId id);
            
            virtual std::string prettyString(Object const& o) const override;
            
            virtual Object new_() const override;
            virtual Object new_(Object* args, std::size_t argCount) const;
            Object new_(double floatval) const;
            
            virtual Object ladd(Object const& lhs, Object const& rhs) const override;
            virtual Object lsub(Object const& lhs, Object const& rhs) const override;
            virtual Object lmul(Object const& lhs, Object const& rhs) const override;
            virtual Object ldiv(Object const& lhs, Object const& rhs) const override;
            virtual Object pow(Object const& lhs, Object const& rhs) const override;
        };
        
        bool isFloat(Object const& obj);
    }
}

#endif // HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2
