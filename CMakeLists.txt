# Copyright (c) <2014-2015> <Florian Erler>
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from
# the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software in a
# product, an acknowledgment in the product documentation would be
# appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such, and must not
# be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.

# Base project setup.
project(nano)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Switch to C++11 mode.
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_definitions("-std=c++11")
    add_definitions("-Wall -Wextra")
endif()

# Build the Bison parser.
find_package(BISON REQUIRED)
set(BisonOutput ${CMAKE_SOURCE_DIR}/Src/Detail/BisonParser.cpp)
if(BISON_FOUND)
    add_custom_command(
      OUTPUT ${BisonOutput}
      COMMAND ${BISON_EXECUTABLE}
              --defines=${CMAKE_SOURCE_DIR}/Include/Nano/Detail/Token.h
              --output=${BisonOutput}
              ${CMAKE_SOURCE_DIR}/Src/Detail/BisonParser.y
      COMMENT "Generating Src/Detail/BisonParser.cpp"
    )
endif()

# Build the Flex lexer.
find_package(FLEX REQUIRED)
set(FlexOutput ${CMAKE_SOURCE_DIR}/Src/Detail/FlexLexer.cpp)
if(FLEX_FOUND)
    add_custom_command(
      OUTPUT ${FlexOutput}
      COMMAND ${FLEX_EXECUTABLE}
              --outfile=${FlexOutput}
              ${CMAKE_SOURCE_DIR}/Src/Detail/FlexLexer.l
      COMMENT "Generating Src/Detail/FlexLexer.cpp"
    )
endif()

# Specify where to to search for header files.
include_directories(
    ${CMAKE_SOURCE_DIR}/Include
    ${CMAKE_SOURCE_DIR}/Lib/rapidxml-1.13/
)

# Compile LibNano.
add_library(nano STATIC
    ${BisonOutput}
    ${FlexOutput}
    Src/Detail/ParseContext.cpp

    Src/Util/RawAllocator.cpp

    Src/Type/IntType.cpp
    Src/Type/RealType.cpp

    Src/Object/Object.cpp
    Src/Object/Int.cpp
    Src/Object/Float.cpp

    Src/Gen/XmlGen.cpp

    Src/VM/VirtualMachine.cpp

    Src/MessageListener.cpp
    Src/ObjectTable.cpp
    Src/GlobalContext.cpp
    Src/Interpreter.cpp
)

# Compile tests.
option (COMPILE_TESTS
   "COMPILE_TESTS" ON)
if(COMPILE_TESTS)
   add_subdirectory(Test)
endif()

# Compile NanoCalc
add_executable(nanocalc
    Src/NanoCalc.cpp
)
target_link_libraries(nanocalc
    nano
    readline
)
