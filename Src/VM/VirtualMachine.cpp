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

// C++ Standard Library:
#include <cmath>

// Nano:
#include <Nano/VM/VirtualMachine.hpp>

nano::vm::VirtualMachine::VirtualMachine()
   : stack_(), frameMarkers(1) {
   frameMarkers[0] = 0;
}

void nano::vm::VirtualMachine::exec(const uint8_t* code) {

#define PUSH_CODEARG(type) stack_.push(*reinterpret_cast<type const*>(code)); \
   code += sizeof(type);

#define CONSUME_CODEARG(type) \
   type codeArg = *reinterpret_cast<type const*>(code); \
   code += sizeof(type);

#define IDUP(type) assert(frameMarkers.size() > 0); \
   CONSUME_CODEARG(StackIndexT) \
   stack_.idup<type>(frameMarkers.back() + codeArg);

#define POP_BINOP_ARGS(type) type rhs = stack_.pop<type>(); \
   type lhs = stack_.pop<type>();

#define SIMPLE_BINOP(type, op) { POP_BINOP_ARGS(type) \
   stack_.push(type(lhs op rhs)); }


start:
   op::OpCode curOpCode = *reinterpret_cast<op::OpCode const*>(code);
   code += sizeof(curOpCode);

   switch(curOpCode) {
      ////////////////////////////////////////////////////////////////
      // VM MANAGEMENT INSTRUCTIONS
      ////////////////////////////////////////////////////////////////
      case op::halt: {
         goto end;
      }

      ////////////////////////////////////////////////////////////////
      // FRAME INSTRUCTIONS
      ////////////////////////////////////////////////////////////////
      case op::framestart: {
         frameMarkers.push_back(stack_.byteSize());
         goto start;
      }
      case op::frameend: {
         assert(frameMarkers.size() > 1);
         frameMarkers.pop_back();
         goto start;
      }

      ////////////////////////////////////////////////////////////////
      // INT INSTRUCTIONS
      ////////////////////////////////////////////////////////////////
      case op::intpush: {
         PUSH_CODEARG(Int)
         goto start;
      }
      case op::intidup: {
         IDUP(Int)
         goto start;
      }
      case op::intfromreal: {
         stack_.push(Int(stack_.pop<Real>()));
         goto start;
      }
      case op::intadd: {
         SIMPLE_BINOP(Int, +)
         goto start;
      }
      case op::intsub: {
         SIMPLE_BINOP(Int, -)
         goto start;
      }
      case op::intmul: {
         SIMPLE_BINOP(Int, *)
         goto start;
      }
      case op::intdiv: {
         SIMPLE_BINOP(Int, /)
         goto start;
      }
      case op::intpow: {
         POP_BINOP_ARGS(Int);
         stack_.push(Int(std::pow(lhs, rhs)));
         goto start;
      }

      ////////////////////////////////////////////////////////////////
      // REAL INSTRUCTIONS
      ////////////////////////////////////////////////////////////////
      case op::realpush: {
         PUSH_CODEARG(Real)
         goto start;
      }
      case op::realidup: {
         IDUP(Real)
         goto start;
      }
      case op::realfromint: {
         stack_.push(Real(stack_.pop<Int>()));
         goto start;
      }
      case op::realadd: {
         SIMPLE_BINOP(Real, +)
         goto start;
      }
      case op::realsub: {
         SIMPLE_BINOP(Real, -)
         goto start;
      }
      case op::realmul: {
         SIMPLE_BINOP(Real, *)
         goto start;
      }
      case op::realdiv: {
         SIMPLE_BINOP(Real, /)
         goto start;
      }
      case op::realpow: {
         POP_BINOP_ARGS(Real);
         stack_.push(Real(std::pow(lhs, rhs)));
         goto start;
      }

      // Should never be reached.
      default:
         assert(false);
   }

end:
return;
}

nano::vm::Stack& nano::vm::VirtualMachine::stack() {
   return stack_;
}
