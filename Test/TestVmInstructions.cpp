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
#include <Nano/VM/VirtualMachine.hpp>
#include <Nano/VM/CodeBuilder.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE VmInstructionTests
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

////////////////////////////////////////////////////////////////
// INT INSTRUCTIONS
////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestIntAdd) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.intpush(5);
   builder.intpush(10);
   builder.intadd();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_EQUAL(15, vm.stack().top<Int>());
}

BOOST_AUTO_TEST_CASE(TestIntSub) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.intpush(10);
   builder.intpush(5);
   builder.intsub();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_EQUAL(5, vm.stack().top<Int>());
}

BOOST_AUTO_TEST_CASE(TestIntMul) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.intpush(5);
   builder.intpush(10);
   builder.intmul();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_EQUAL(50, vm.stack().top<Int>());
}

BOOST_AUTO_TEST_CASE(TestIntDiv) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.intpush(10);
   builder.intpush(5);
   builder.intdiv();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_EQUAL(2, vm.stack().top<Int>());
}

BOOST_AUTO_TEST_CASE(TestIntPow) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.intpush(5);
   builder.intpush(3);
   builder.intpow();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_EQUAL(125, vm.stack().top<Int>());
}

////////////////////////////////////////////////////////////////
// REAL INSTRUCTIONS
////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(TestRealAdd) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.realpush(1.5);
   builder.realpush(10.2);
   builder.realadd();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_CLOSE(11.7, vm.stack().top<Real>(), 0.001);
}

BOOST_AUTO_TEST_CASE(TestRealSub) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.realpush(11.6);
   builder.realpush(1.6);
   builder.realsub();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_CLOSE(10.0, vm.stack().top<Real>(), 0.001);
}

BOOST_AUTO_TEST_CASE(TestRealMul) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.realpush(2.5);
   builder.realpush(2.5);
   builder.realmul();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_CLOSE(6.25, vm.stack().top<Real>(), 0.001);
}

BOOST_AUTO_TEST_CASE(TestRealDiv) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.realpush(10.5);
   builder.realpush(2.0);
   builder.realdiv();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_CLOSE(5.25, vm.stack().top<Real>(), 0.001);
}

BOOST_AUTO_TEST_CASE(TestRealPow) {
   using namespace nano::vm;

   CodeBuilder builder;
   builder.realpush(2.5);
   builder.realpush(2.0);
   builder.realpow();
   builder.halt();

   VirtualMachine vm;
   vm.exec(builder.buffer().code());
   BOOST_REQUIRE_CLOSE(6.25, vm.stack().top<Real>(), 0.001);
}