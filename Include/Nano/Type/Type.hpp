// Copyright (c) <2015> <Florian Erler>
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

#pragma once

// C++ Standard Library:
#include <string>
#include <memory>

namespace nano {
   namespace type {
      typedef unsigned TypeId;

      TypeId const TYPEID_INT = 1;
      TypeId const TYPEID_REAL = 2;

      class Type {
      public:
         typedef std::shared_ptr<Type> PtrT;

         template<typename T, typename... Args>
         static PtrT make(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);
         }

      public:
         virtual ~Type() = default;
         virtual TypeId typeId() = 0;
         virtual PtrT commonType(PtrT const& other) = 0;
         virtual bool isPrimitive() = 0;
         virtual std::string const& typeString() = 0;

         virtual PtrT resultTypeOfAddition(PtrT const& rhs) = 0;
         virtual PtrT resultTypeOfSubtraction(PtrT const& rhs) = 0;
         virtual PtrT resultTypeOfMultiplication(PtrT const& rhs) = 0;
         virtual PtrT resultTypeOfDivision(PtrT const& rhs) = 0;
         virtual PtrT resultTypeOfPotentiation(PtrT const& rhs) = 0;
      };
   }
}