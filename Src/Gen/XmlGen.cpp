// Copyright (c) <2014> <Florian Erler>
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

class XmlGen : public Visitor
{
private:
    xml_document<> _doc;
    std::vector<xml_node<>*> _stack;
    
public:
    // Value AST:
    virtual void accept(IntNode* n) override
    {
        xml_node<>* xn = _doc.allocate_node(node_element, "IntNode");
        char* value = _doc.allocate_string(n->value().str().c_str());
        xml_attribute<>* valueAttr = _doc.allocate_attribute("value", value);
        xn->append_attribute(valueAttr);
        _stack.push_back(xn);
    }
    
    virtual void accept(FloatNode* n) override
    {
        xml_node<>* xn = _doc.allocate_node(node_element, "FloatNode");
        char* value = _doc.allocate_string(n->value().str(0, std::ios_base::fixed).c_str());
        xml_attribute<>* valueAttr = _doc.allocate_attribute("value", value);
        xn->append_attribute(valueAttr);
        _stack.push_back(xn);
    }
    
    virtual void accept(VarNode* n) override
    {
        xml_node<>* xn = _doc.allocate_node(node_element, "VarNode");
        xml_attribute<>* valueAttr = _doc.allocate_attribute("value", n->value().c_str());
        xn->append_attribute(valueAttr);
        _stack.push_back(xn);
    }
            
    // Binary Arithmetic AST:
    virtual void accept(BinaryPlusNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "BinaryPlusNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
    
    virtual void accept(BinaryMinusNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "BinaryMinusNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
    
    virtual void accept(BinaryMultiplyNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "BinaryMultiplyNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
    
    virtual void accept(BinaryDivideNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "BinaryDivideNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
    
    virtual void accept(BinaryPowerNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "BinaryPowerNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
    
    // Assignment AST:
    virtual void accept(AssignmentNode* n) override
    {
        n->rhs()->visit(this);
        n->lhs()->visit(this);
        
        xml_node<>* xn = _doc.allocate_node(node_element, "AssignmentNode");
        xn->append_node(_stack.back());
        _stack.pop_back();
        xn->append_node(_stack.back());
        _stack.pop_back();
        _stack.push_back(xn);
    }
            
    // Other Expression AST:
    virtual void accept(CallNode* n) override
    {
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
    
    // Other:
    virtual void accept(ExpressionListNode* n) override
    {
        xml_node<>* xn = _doc.allocate_node(node_element, "ExpressionListNode");
        for(Node::PtrT& cur : n->list())
        {
            cur->visit(this);
            xn->append_node(_stack.back());
            _stack.pop_back();
        }
        _stack.push_back(xn);
    }
    
    xml_node<>* rootNode()
    {
        assert(_stack.size() == 1);
        return _stack.back();
    }
};

void nano::gen::genXml(std::ostream& o, ast::Node* ast)
{
    XmlGen gen;
    ast->visit(&gen);
    print(o, *gen.rootNode());
}