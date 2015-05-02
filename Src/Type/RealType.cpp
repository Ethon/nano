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

// Nano:
#include <Nano/Type/RealType.hpp>

using namespace nano::type;

Type::PtrT RealType::instance = Type::make<RealType>();

TypeId RealType::typeId() {
   return TYPEID_REAL;
}

Type::PtrT RealType::commonType(PtrT const& other) {
   if(other->typeId() == TYPEID_INT || other->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}

bool RealType::isPrimitive() {
   return true;
}

std::string const& RealType::typeString() {
   static std::string result = "real";
   return result;
}

Type::PtrT RealType::resultTypeOfAddition(const Type::PtrT& rhs) {
   if(rhs->typeId() == TYPEID_INT || rhs->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}

Type::PtrT RealType::resultTypeOfSubtraction(PtrT const& rhs) {
   if(rhs->typeId() == TYPEID_INT || rhs->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}

Type::PtrT RealType::resultTypeOfMultiplication(PtrT const& rhs) {
   if(rhs->typeId() == TYPEID_INT || rhs->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}

Type::PtrT RealType::resultTypeOfDivision(PtrT const& rhs) {
   if(rhs->typeId() == TYPEID_INT || rhs->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}

Type::PtrT RealType::resultTypeOfPotentiation(PtrT const& rhs) {
   if(rhs->typeId() == TYPEID_INT || rhs->typeId() == TYPEID_REAL) {
      return instance;
   }
   return Type::PtrT();
}
