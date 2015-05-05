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
#include <vector>
#include <type_traits>
#include <cassert>

// Nano:
#include <Nano/VM/OpCode.hpp>
#include <Nano/VM/Types.hpp>

namespace nano {
   namespace vm {
      class Stack {
      private:
         std::vector<std::uint8_t> stack;

      public:
         template<typename T>
         void push(T value) {
            static_assert(std::is_pod<T>::value, "Only POD types permitted");
            std::uint8_t* valueBytes = reinterpret_cast<std::uint8_t*>(&value);
            stack.insert(stack.end(), valueBytes, valueBytes + sizeof(value));
         }

         template<typename T>
         T pop() {
            static_assert(std::is_pod<T>::value, "Only POD types permitted");
            assert(stack.size() >= sizeof(T));
            T result = top<T>();
            stack.resize(stack.size() - sizeof(T));
            return result;
         }

         template<typename T>
         T top() const {
            static_assert(std::is_pod<T>::value, "Only POD types permitted");
            assert(stack.size() >= sizeof(T));
            T result = *reinterpret_cast<T const*>(
               &stack[stack.size() - sizeof(T)]);
            return result;
         }

         template<typename T>
         void idup(std::size_t index) {
            static_assert(std::is_pod<T>::value, "Only POD types permitted");
            assert(stack.size() >= sizeof(T));
            assert(index >= 0 && (stack.size() - sizeof(T)) >= index);
            T val = *reinterpret_cast<T const*>(
               &stack[index]);
            push(val);
         }

         std::size_t byteSize() const {
            return stack.size();
         }
      };
   }
}