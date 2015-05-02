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
#include <ostream>

// Nano:
#include <Nano/Common/SourcePos.hpp>
#include <Nano/Type/Type.hpp>

namespace nano {
   class MessageListener {
   public:
      virtual ~MessageListener() = default;

      // Error class.
      virtual void onBinaryOperationError(SourcePos const& pos,
         type::Type::PtrT const& lhsType, type::Type::PtrT const& rhsType,
         std::string const& msg) = 0;
   };
   
   class StreamWritingListener : public MessageListener {
   private:
      std::ostream& out;

   public:
      explicit StreamWritingListener(std::ostream& out);

      virtual void onBinaryOperationError(SourcePos const& pos,
         type::Type::PtrT const& lhsType, type::Type::PtrT const& rhsType,
         std::string const& msg) override;
   };
}