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
    
    Src/Object/Object.cpp
    Src/Object/IntClass.cpp
    Src/Object/FloatClass.cpp
    Src/Object/NativeFunctionClass.cpp
    
    Src/Gen/XmlGen.cpp
    
    Src/ObjectTable.cpp
    Src/GlobalContext.cpp
    Src/Interpreter.cpp
)

# Compile NanoCalc
add_executable(nanocalc
    Src/NanoCalc.cpp
)
target_link_libraries(nanocalc
    nano
    readline
)