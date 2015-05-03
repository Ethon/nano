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
#include <iostream>
#include <string>
#include <fstream>

// Nano:
#include <Nano/Detail/ParseContext.hpp>
#include <Nano/Gen/XmlGen.hpp>

int main(int argc, char** argv) {
   if(argc < 2 || argc > 3) {
      std::cerr << "Usage: " << argv[0] << " <sourcefile> [<outputfile>]\n";
      return -1;
   }
   std::string sourceFile = argv[1];
   std::string outputFile;
   if(argc == 3) {
      outputFile = argv[2];
   } else {
      outputFile = sourceFile + ".xml";
   }

   nano::detail::ParseContext ctx(sourceFile.c_str(),
                                  nano::detail::ParseContext::FileInput());
   auto ast = ctx.parseOne();
   if(!ast) {
      std::cerr << "nanox: Aborted due to parsing errors\n";
      return -2;
   }
   std::ofstream out(outputFile);
   if(!out) {
      std::cerr << "nanox: Could not open output file\n";
      return -3;
   }
   nano::gen::genXml(out, ast.get());
}