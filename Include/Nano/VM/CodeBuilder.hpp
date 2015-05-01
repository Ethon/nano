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

// Nano:
#include <Nano/VM/CodeBuffer.hpp>
#include <Nano/VM/OpCode.hpp>
#include <Nano/VM/Types.hpp>

namespace nano {
   namespace vm {
      class CodeBuilder {
      private:
         CodeBuffer buffer_;

      public:
         inline CodeBuffer const& buffer() const {
            return buffer_;
         }

         ////////////////////////////////////////////////////////////////
         // VM MANAGEMENT INSTRUCTIONS
         ////////////////////////////////////////////////////////////////
         inline void halt() {
            buffer_.put(op::halt);
         }

         ////////////////////////////////////////////////////////////////
         // INT INSTRUCTIONS
         ////////////////////////////////////////////////////////////////
         inline void intpush(Int value) {
            buffer_.put(op::intpush);
            buffer_.put(value);
         }

         inline void intadd() {
            buffer_.put(op::intadd);
         }

         inline void intsub() {
            buffer_.put(op::intsub);
         }

         inline void intmul() {
            buffer_.put(op::intmul);
         }

         inline void intdiv() {
            buffer_.put(op::intdiv);
         }

         inline void intpow() {
            buffer_.put(op::intpow);
         }

         ////////////////////////////////////////////////////////////////
         // REAL INSTRUCTIONS
         ////////////////////////////////////////////////////////////////
         inline void realpush(Real value) {
            buffer_.put(op::realpush);
            buffer_.put(value);
         }

         inline void realadd() {
            buffer_.put(op::realadd);
         }

         inline void realsub() {
            buffer_.put(op::realsub);
         }

         inline void realmul() {
            buffer_.put(op::realmul);
         }

         inline void realdiv() {
            buffer_.put(op::realdiv);
         }

         inline void realpow() {
            buffer_.put(op::realpow);
         }
      };
   }
}