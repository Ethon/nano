// Copyright (c) <2014-2015> <Florian Erler>
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
#include <vector>

// RapidXml:
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// Nano:
#include <Nano/Gen/XmlGen.hpp>
#include <Nano/Ast/All.hpp>
        
using namespace nano;
using namespace nano::ast;
using namespace rapidxml;

class XmlGen : public Visitor {
private:
   xml_document<> _doc;
   std::vector<xml_node<>*> _stack;

   void genValue(char const* nodeName, char const* value_) {
      xml_node<>* xn = _doc.allocate_node(node_element, nodeName);
      char* value = _doc.allocate_string(value_);
      xml_attribute<>* valueAttr = _doc.allocate_attribute("value", value);
      xn->append_attribute(valueAttr);
      _stack.push_back(xn);
   }

   template<typename NodeT>
   void genBinary(NodeT* n, char const* nodeName) {
      n->rhs()->visit(this);
      n->lhs()->visit(this);

      xml_node<>* xn = _doc.allocate_node(node_element, nodeName);
      xn->append_node(_stack.back());
      _stack.pop_back();
      xn->append_node(_stack.back());
      _stack.pop_back();
      _stack.push_back(xn);
   }

   template<typename IterT>
   void visitList(IterT begin, IterT end, char const* nodeName) {
      xml_node<>* xn = _doc.allocate_node(node_element, nodeName);
      for(; begin != end; ++begin) {
         (*begin)->visit(this);
         xn->append_node(_stack.back());
         _stack.pop_back();
      }
      _stack.push_back(xn);
   }

public:
   // Value AST:
   virtual void accept(IntNode* n) override {
      genValue("IntNode", n->value().str().c_str());
   }

   virtual void accept(FloatNode* n) override {
      genValue("FloatNode", n->value().str(0, std::ios_base::fixed).c_str());
   }

   virtual void accept(VarNode* n) override {
      genValue("VarNode", n->value().c_str());
   }

   // Binary Arithmetic AST:
   virtual void accept(BinaryPlusNode* n) override {
      genBinary(n, "BinaryPlusNode");
   }

   virtual void accept(BinaryMinusNode* n) override {
      genBinary(n, "BinaryMinusNode");
   }

   virtual void accept(BinaryMultiplyNode* n) override {
      genBinary(n, "BinaryMultiplyNode");
   }

   virtual void accept(BinaryDivideNode* n) override {
      genBinary(n, "BinaryDivideNode");
   }

   virtual void accept(BinaryPowerNode* n) override {
      genBinary(n, "BinaryPowerNode");
   }

   // Assignment AST:
   virtual void accept(AssignmentNode* n) override {
      genBinary(n, "AssignmentNode");
   }

   // Other Expression AST:
   virtual void accept(CallNode* n) override {
      xml_node<>* call = _doc.allocate_node(node_element, "CallNode");

      xml_node<>* target = _doc.allocate_node(node_element, "CallTarget");
      n->target()->visit(this);
      target->append_node(_stack.back());
      _stack.pop_back();
      call->append_node(target);

      xml_node<>* args = _doc.allocate_node(node_element, "CallArgs");
      n->args()->visit(this);
      args->append_node(_stack.back());
      _stack.pop_back();
      call->append_node(args);

      _stack.push_back(call);
   }

   // Statement AST:
   virtual void accept(CodeBlockNode* n) override {
      auto list = n->list();
      visitList(list.begin(), list.end(), "CodeBlockNode");
   }

   virtual void accept(FunctionDefinitionNode* n) override {
      xml_node<>* def = _doc.allocate_node(node_element,
                                           "FunctionDefinitionNode");

      // Generate argument list.
      xml_node<>* args = _doc.allocate_node(node_element,
                                              "FunctionArguments");
      n->arglist()->visit(this);
      args->append_node(_stack.back());
      _stack.pop_back();
      def->append_node(args);

      // Generate body.
      xml_node<>* code = _doc.allocate_node(node_element, "Code");
      n->code()->visit(this);
      code->append_node(_stack.back());
      _stack.pop_back();
      def->append_node(code);

      _stack.push_back(def);
   }

   // Other:
   virtual void accept(ExpressionListNode* n) override {
      auto list = n->list();
      visitList(list.begin(), list.end(), "ExpressionListNode");
   }

   virtual void accept(TypeNode* n) override {
      genValue("TypeNode", n->typeString().c_str());
   }

   virtual void accept(VariableDeclerationNode* n) override {
      xml_node<>* xn = _doc.allocate_node(node_element,
                                          "VariableDeclerationNode");

      // Generate the varname.
      xml_attribute<>* nameAttr = _doc.allocate_attribute("name",
                                                          n->varName().c_str());
      xn->append_attribute(nameAttr);

      // Generate the type.
      n->type()->visit(this);
      xn->append_node(_stack.back());
      _stack.pop_back();

      _stack.push_back(xn);
   }

   virtual void accept(VariableDeclerationListNode* n) override {
      auto list = n->list();
      visitList(list.begin(), list.end(), "VariableDeclerationListNode");
   }

   xml_node<>* rootNode() {
      assert(_stack.size() == 1);
      return _stack.back();
   }
};

void nano::gen::genXml(std::ostream& o, ast::Node* ast) {
   XmlGen gen;
   ast->visit(&gen);
   print(o, *gen.rootNode());
}