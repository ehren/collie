
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <cassert>
#include <compare> // for <=>
#include <thread>
#include <optional>

//#include <concepts>
//#include <ranges>
//#include <numeric>


#include "ceto.h"


#include <map>
;
#include <typeinfo>
;
#include <numeric>
;
#include <pybind11/pybind11.h>
;
#include <pybind11/stl.h>
;
#include <pybind11/stl_bind.h>
;
#include <pybind11/functional.h>
;
#include "ast.donotedit.autogenerated.h"
;
#include "repr_visitors.donotedit.autogenerated.h"
;
#include "scope.donotedit.autogenerated.h"
;
#include "parser.donotedit.autogenerated.h"
;
#include "macro_matcher.donotedit.autogenerated.h"
;
#include "macro_definition_visitor.donotedit.autogenerated.h"
;
namespace py = pybind11;
    inline auto module_path() -> auto {
        const auto acquire = py::gil_scoped_acquire();
        const py::object module = py::module::import("ceto"); static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(py::module::import("ceto")), std::remove_cvref_t<decltype(module)>>);
        const py::object os = py::module::import("os"); static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(py::module::import("os")), std::remove_cvref_t<decltype(os)>>);
        const auto exeloc = ceto::mado(module)->attr("__file__");
        const auto dir = ceto::mado(ceto::mado(os)->attr("path"))->attr(exeloc);
        return ceto::mado(dir)->cast<std::string>();
    }


PYBIND11_MODULE(_abstractsyntaxtree, m) {
;
[]( auto &&  m) {
using namespace pybind11::literals;
        auto node { ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<Node,std::shared_ptr<Node>>(m, "Node"))->def_readwrite("func", (&Node::func)))->def_readwrite("args", (&Node::args)))->def_readwrite("declared_type", (&Node::declared_type)))->def_readwrite("scope", (&Node::scope)))->def_readwrite("source", (&Node::source)))->def("__repr__", (&Node::repr)))->def("ast_repr", [](const Node&  n, const bool  preserve_source_loc, const bool  ceto_evalable) {
                auto vis { EvalableAstReprVisitor{preserve_source_loc, ceto_evalable} } ;
                ceto::mado(n)->accept(vis);
                if constexpr (!std::is_void_v<decltype(ceto::mado(vis)->repr)>) { return ceto::mado(vis)->repr; } else { static_cast<void>(ceto::mado(vis)->repr); };
                }, py::arg("preserve_source_loc") = true, py::arg("ceto_evalable") = false))->def_property_readonly("name", (&Node::name)))->def_property("parent", (&Node::parent), (&Node::set_parent)))->def_readwrite("file_path", (&Node::file_path)) } ;
        ceto::mado(ceto::mado(py::class_<UnOp,std::shared_ptr<UnOp>>(m, "UnOp", node))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("op", (&UnOp::op));
        ceto::mado(ceto::mado(py::class_<LeftAssociativeUnOp,std::shared_ptr<LeftAssociativeUnOp>>(m, "LeftAssociativeUnOp", node))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("op", (&LeftAssociativeUnOp::op));
        auto binop { py::class_<BinOp,std::shared_ptr<BinOp>>(m, "BinOp", node) } ;
        ceto::mado(ceto::mado(ceto::mado(ceto::mado(binop)->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("op", (&BinOp::op)))->def_property_readonly("lhs", (&BinOp::lhs)))->def_property_readonly("rhs", (&BinOp::rhs));
        auto typeop { py::class_<TypeOp,std::shared_ptr<TypeOp>>(m, "TypeOp", binop) } ;
        ceto::mado(typeop)->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(ceto::mado(py::class_<SyntaxTypeOp,std::shared_ptr<SyntaxTypeOp>>(m, "SyntaxTypeOp", typeop))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("synthetic_lambda_return_lambda", (&SyntaxTypeOp::synthetic_lambda_return_lambda));
        ceto::mado(py::class_<AttributeAccess,std::shared_ptr<AttributeAccess>>(m, "AttributeAccess", binop))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<ArrowOp,std::shared_ptr<ArrowOp>>(m, "ArrowOp", binop))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<ScopeResolution,std::shared_ptr<ScopeResolution>>(m, "ScopeResolution", binop))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        auto assign { py::class_<Assign,std::shared_ptr<Assign>>(m, "Assign", binop) } ;
        ceto::mado(assign)->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<NamedParameter,std::shared_ptr<NamedParameter>>(m, "NamedParameter", assign))->def(py::init<const std::string &,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("op"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(ceto::mado(py::class_<Call,std::shared_ptr<Call>>(m, "Call", node))->def(py::init<std::shared_ptr<const Node>,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("func"), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("is_one_liner_if", (&Call::is_one_liner_if));
        ceto::mado(py::class_<ArrayAccess,std::shared_ptr<ArrayAccess>>(m, "ArrayAccess", node))->def(py::init<std::shared_ptr<const Node>,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("func"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<BracedCall,std::shared_ptr<BracedCall>>(m, "BracedCall", node))->def(py::init<std::shared_ptr<const Node>,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("func"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<Template,std::shared_ptr<Template>>(m, "Template", node))->def(py::init<std::shared_ptr<const Node>,std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("func"), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<Identifier,std::shared_ptr<Identifier>>(m, "Identifier", node))->def(py::init<const std::string &,std::tuple<std::string,int>>(), py::arg("name"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<StringLiteral,std::shared_ptr<StringLiteral>>(m, "StringLiteral", node))->def(py::init<const std::string &,std::shared_ptr<const Identifier>,std::shared_ptr<const Identifier>,std::tuple<std::string,int>>(), py::arg("str"), py::arg("prefix"), py::arg("suffix"), py::arg("source") = std::make_tuple("", 0)))->def_readonly("str", (&StringLiteral::str)))->def_readwrite("prefix", (&StringLiteral::prefix)))->def_readwrite("suffix", (&StringLiteral::suffix)))->def("escaped", (&StringLiteral::escaped));
        ceto::mado(ceto::mado(ceto::mado(py::class_<IntegerLiteral,std::shared_ptr<IntegerLiteral>>(m, "IntegerLiteral", node))->def(py::init<const std::string &,std::shared_ptr<const Identifier>,std::tuple<std::string,int>>(), py::arg("integer_string"), py::arg("suffix"), py::arg("source") = std::make_tuple("", 0)))->def_readonly("integer_string", (&IntegerLiteral::integer_string)))->def_readonly("suffix", (&IntegerLiteral::suffix));
        ceto::mado(ceto::mado(ceto::mado(py::class_<FloatLiteral,std::shared_ptr<FloatLiteral>>(m, "FloatLiteral", node))->def(py::init<const std::string &,std::shared_ptr<const Identifier>,std::tuple<std::string,int>>(), py::arg("float_string"), py::arg("suffix"), py::arg("source") = std::make_tuple("", 0)))->def_readonly("float_string", (&FloatLiteral::float_string)))->def_readonly("suffix", (&FloatLiteral::suffix));
        auto list_like { py::class_<ListLike_,std::shared_ptr<ListLike_>>(m, "ListLike_", node) } ;
        ceto::mado(py::class_<ListLiteral,std::shared_ptr<ListLiteral>>(m, "ListLiteral", list_like))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<TupleLiteral,std::shared_ptr<TupleLiteral>>(m, "TupleLiteral", list_like))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<BracedLiteral,std::shared_ptr<BracedLiteral>>(m, "BracedLiteral", list_like))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        auto block { py::class_<Block,std::shared_ptr<Block>>(m, "Block", list_like) } ;
        ceto::mado(block)->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(ceto::mado(py::class_<Module,std::shared_ptr<Module>>(m, "Module", block))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0)))->def_readwrite("has_main_function", (&Module::has_main_function));
        ceto::mado(py::class_<RedundantParens,std::shared_ptr<RedundantParens>>(m, "RedundantParens", node))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        ceto::mado(py::class_<InfixWrapper_,std::shared_ptr<InfixWrapper_>>(m, "InfixWrapper_", node))->def(py::init<std::vector<std::shared_ptr<const Node>>,std::tuple<std::string,int>>(), py::arg("args"), py::arg("source") = std::make_tuple("", 0));
        auto class_def { ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<ClassDefinition,std::shared_ptr<ClassDefinition>>(m, "ClassDefinition"))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Call>,bool,bool,bool>(), py::arg("name_node"), py::arg("class_def_node"), py::arg("is_unique"), py::arg("is_struct"), py::arg("is_forward_declaration")))->def_readwrite("name_node", (&ClassDefinition::name_node)))->def_readwrite("class_def_node", (&ClassDefinition::class_def_node)))->def_readwrite("is_unique", (&ClassDefinition::is_unique)))->def_readwrite("is_struct", (&ClassDefinition::is_struct)))->def_readwrite("is_forward_declaration", (&ClassDefinition::is_forward_declaration)))->def_readwrite("is_concrete", (&ClassDefinition::is_concrete)))->def_readwrite("is_pure_virtual", (&ClassDefinition::is_pure_virtual)))->def("__repr__", (&ClassDefinition::repr)) } ;
        ceto::mado(py::class_<InterfaceDefinition,std::shared_ptr<InterfaceDefinition>>(m, "InterfaceDefinition", class_def))->def(py::init<>());
        auto variable_def { ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<VariableDefinition,std::shared_ptr<VariableDefinition>>(m, "VariableDefinition"))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Node>>(), py::arg("defined_node"), py::arg("defining_node")))->def_readwrite("defined_node", (&VariableDefinition::defined_node)))->def_readwrite("defining_node", (&VariableDefinition::defining_node)))->def("__repr__", (&VariableDefinition::repr)) } ;
        ceto::mado(py::class_<LocalVariableDefinition,std::shared_ptr<LocalVariableDefinition>>(m, "LocalVariableDefinition", variable_def))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Node>>(), py::arg("defined_node"), py::arg("defining_node"));
        ceto::mado(py::class_<GlobalVariableDefinition,std::shared_ptr<GlobalVariableDefinition>>(m, "GlobalVariableDefinition", variable_def))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Node>>(), py::arg("defined_node"), py::arg("defining_node"));
        ceto::mado(py::class_<FieldDefinition,std::shared_ptr<FieldDefinition>>(m, "FieldDefinition", variable_def))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Node>>(), py::arg("defined_node"), py::arg("defining_node"));
        ceto::mado(py::class_<ParameterDefinition,std::shared_ptr<ParameterDefinition>>(m, "ParameterDefinition", variable_def))->def(py::init<std::shared_ptr<const Identifier>,std::shared_ptr<const Node>>(), py::arg("defined_node"), py::arg("defining_node"));
        ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<Scope,std::shared_ptr<Scope>>(m, "Scope"))->def(py::init<>()))->def_readwrite("indent", (&Scope::indent)))->def_readwrite("in_function_body", (&Scope::in_function_body)))->def_readwrite("in_function_param_list", (&Scope::in_function_param_list)))->def_readwrite("in_class_body", (&Scope::in_class_body)))->def_readwrite("in_decltype", (&Scope::in_decltype)))->def("indent_str", (&Scope::indent_str)))->def("add_variable_definition", (&Scope::add_variable_definition), "defined_node"_a, "defining_node"_a))->def("add_interface_method", (&Scope::add_interface_method)))->def("add_class_definition", (&Scope::add_class_definition)))->def("lookup_class", (&Scope::lookup_class)))->def("find_defs", (&Scope::find_defs), py::arg("var_node"), py::arg("find_all") = true))->def("find_def", (&Scope::find_def)))->def("enter_scope", (&Scope::enter_scope)))->def_property_readonly("parent", (&Scope::parent));
        ceto::mado(m)->def("creates_new_variable_scope", (&creates_new_variable_scope));
        ceto::mado(m)->def("parse_test", (&parse_test));
        py::bind_map<std::map<std::string,std::shared_ptr<const Node>>>(m, "StringNodeMap");
        ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(ceto::mado(py::class_<MacroDefinition,std::shared_ptr<MacroDefinition>>(m, "MacroDefinition"))->def(py::init<std::shared_ptr<const Call>,std::shared_ptr<const Node>,std::shared_ptr<const Block>,std::map<std::string,std::shared_ptr<const Node>>>()))->def_readonly("defmacro_node", (&MacroDefinition::defmacro_node)))->def_readonly("pattern_node", (&MacroDefinition::pattern_node)))->def_readonly("body", (&MacroDefinition::body)))->def_readonly("parameters", (&MacroDefinition::parameters)))->def_readwrite("dll_path", (&MacroDefinition::dll_path)))->def_readwrite("impl_function_name", (&MacroDefinition::impl_function_name));
        ceto::mado(ceto::mado(ceto::mado(py::class_<MacroScope,std::shared_ptr<MacroScope>>(m, "MacroScope"))->def(py::init<>()))->def_property_readonly("parent", (&MacroScope::parent)))->def_readwrite("macro_definitions", (&MacroScope::macro_definitions));
        ceto::mado(m)->def("macro_matches", (&macro_matches));
        ceto::mado(m)->def("visit_macro_definitions", (&visit_macro_definitions));
        ceto::mado(m)->def("macro_trampoline", (&macro_trampoline));
        return;
        }(m);
};
