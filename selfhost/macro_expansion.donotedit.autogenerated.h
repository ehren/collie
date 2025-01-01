#pragma once

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


#include "ceto.h"
//#include "ceto_private_boundscheck.donotedit.autogenerated.h"

#include "ceto_private_listcomp.donotedit.autogenerated.h"
;
#include "ceto_private_boundscheck.donotedit.autogenerated.h"
;
#include "ceto_private_convenience.donotedit.autogenerated.h"
;
#include <map>
;
#include <unordered_map>
;
#include <ranges>
;
#include <functional>
;
#include <span>
;
#include <variant>
;
#include "ast.donotedit.autogenerated.h"
;
#include "visitor.donotedit.autogenerated.h"
;
#include "range_utility.donotedit.autogenerated.h"
;
#if _MSC_VER
    #include <windows.h>
;
    
    #define CETO_DLSYM GetProcAddress
    #define CETO_DLOPEN LoadLibraryA
    #define CETO_DLCLOSE FreeLibrary
    ;
#else
    #include <dlfcn.h>
;
    
    #define CETO_DLSYM dlsym
    #define CETO_DLOPEN(L) dlopen(L, RTLD_NOW)
    #define CETO_DLCLOSE dlclose
    ;
#endif

struct SemanticAnalysisError : public std::runtime_error {

using std::runtime_error::runtime_error;

};

struct MacroDefinition : public ceto::shared_object, public std::enable_shared_from_this<MacroDefinition> {

    std::shared_ptr<const Node> defmacro_node;

    std::shared_ptr<const Node> pattern_node;

    std::map<std::string,std::shared_ptr<const Node>> parameters;

    std::string dll_path = {};

    std::string impl_function_name = {};

    explicit MacroDefinition(std::shared_ptr<const Node> defmacro_node, std::shared_ptr<const Node> pattern_node, std::map<std::string,std::shared_ptr<const Node>> parameters) : defmacro_node(std::move(defmacro_node)), pattern_node(std::move(pattern_node)), parameters(parameters) {}

    MacroDefinition() = delete;

};

struct MacroScope : public ceto::object {

    MacroScope const * parent = nullptr; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(nullptr), std::remove_cvref_t<decltype(parent)>>);

    std::vector<std::shared_ptr<const MacroDefinition>> macro_definitions = std::vector<std::shared_ptr<const MacroDefinition>>{}; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::vector<std::shared_ptr<const MacroDefinition>>{}), std::remove_cvref_t<decltype(macro_definitions)>>);

        inline auto add_definition(const std::shared_ptr<const MacroDefinition>&  defn) -> void {
            (*ceto::mad(this -> macro_definitions)).push_back(defn);
        }

        inline auto enter_scope() const -> std::unique_ptr<MacroScope> {
            auto s = std::make_unique<MacroScope>();
            (*ceto::mad(s)).parent = this;
            return s;
        }

};

    inline auto macro_matches(const std::shared_ptr<const Node>&  node, const std::shared_ptr<const Node>&  pattern,  const std::map<std::string,std::shared_ptr<const Node>> &  params) -> std::optional<std::map<std::string,std::shared_ptr<const Node>>> {
        if ((std::dynamic_pointer_cast<const Identifier>(pattern) != nullptr)) {
            const auto search = (*ceto::mad(params)).find((*ceto::mad_smartptr((*ceto::mad(pattern)).name())).value());
            if (search != (*ceto::mad(params)).end()) {
                const auto param_name = (search -> first);
                const auto matched_param = (search -> second);
                if ((std::dynamic_pointer_cast<const Identifier>(matched_param) != nullptr)) {
                    return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                } else if (const auto typeop = std::dynamic_pointer_cast<const TypeOp>(matched_param)) {
                    const auto param_type = (*ceto::mad(typeop)).rhs();
                    if ((std::dynamic_pointer_cast<const Identifier>(param_type) != nullptr)) {
                        if ((((((*ceto::mad(param_type)).name() == "BinOp") && (std::dynamic_pointer_cast<const BinOp>(node) != nullptr)) || (((*ceto::mad(param_type)).name() == "UnOp") && (std::dynamic_pointer_cast<const UnOp>(node) != nullptr))) || ((*ceto::mad(param_type)).name() == "Node")) || ((*ceto::mad(node)).classname() == (*ceto::mad((*ceto::mad(typeop)).rhs())).name())) {
                            return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                        }
                    } else if (const auto or_type = std::dynamic_pointer_cast<const BitwiseOrOp>(param_type)) {
                        const std::map<std::string,std::shared_ptr<const Node>> lhs_alternate_param = {{param_name, std::make_shared<const TypeOp>(":", std::vector {{matched_param, (*ceto::mad(or_type)).lhs()}})}};
                        if (const auto m = macro_matches(node, pattern, lhs_alternate_param)) {
                            return m;
                        }
                        const std::map<std::string,std::shared_ptr<const Node>> rhs_alternate_param = {{param_name, std::make_shared<const TypeOp>(":", std::vector {{matched_param, (*ceto::mad(or_type)).rhs()}})}};
                        if (const auto m = macro_matches(node, pattern, rhs_alternate_param)) {
                            return m;
                        }
                    }
                }
            }
        } else if (const auto binop_pattern = std::dynamic_pointer_cast<const BinOp>(pattern)) {
            std::vector<std::shared_ptr<const Node>> idents = std::vector<std::shared_ptr<const Node>>{}; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::vector<std::shared_ptr<const Node>>{}), std::remove_cvref_t<decltype(idents)>>);
            for(const auto& a : (*ceto::mad(binop_pattern)).args) {
                if ((std::dynamic_pointer_cast<const Identifier>(a) != nullptr)) {
                    (*ceto::mad(idents)).push_back(a);
                }
            }
            for(const auto& i : idents) {
                const auto search = (*ceto::mad(params)).find((*ceto::mad_smartptr((*ceto::mad(i)).name())).value());
                if (search != (*ceto::mad(params)).end()) {
                    const auto param_name = (search -> first);
                    const auto typed_param = std::dynamic_pointer_cast<const TypeOp>(search -> second);
                    if (!typed_param) {
                        continue;
                    }
                    if (const auto or_param = std::dynamic_pointer_cast<const BitwiseOrOp>((*ceto::mad(typed_param)).rhs())) {
                        if (((*ceto::mad((*ceto::mad(or_param)).lhs())).name() == "None") || ((*ceto::mad((*ceto::mad(or_param)).rhs())).name() == "None")) {
                            for(const auto& a : (*ceto::mad(binop_pattern)).args) {
                                if ((*ceto::mad(a)).name() != (*ceto::mad(i)).name()) {
                                    const auto m = macro_matches(node, a, params);
                                    if (m) {
                                        return m;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (typeid((*node)) != typeid((*pattern))) {
            return {};
        }
        if (((*ceto::mad(node)).func == nullptr) != ((*ceto::mad(pattern)).func == nullptr)) {
            return {};
        }
        if ((((*ceto::mad((*ceto::mad(node)).args)).size() == 0) && ((*ceto::mad(node)).func == nullptr)) && ((*ceto::mad(pattern)).func == nullptr)) {
            if ((*ceto::mad(node)).repr() == (*ceto::mad(pattern)).repr()) {
                return std::map<std::string,std::shared_ptr<const Node>>{};
            }
            return {};
        }
        auto submatches { std::map<std::string,std::shared_ptr<const Node>>{} } ;
        if ((*ceto::mad(node)).func) {
            const auto m = macro_matches((*ceto::mad(node)).func, (*ceto::mad(pattern)).func, params);
            if (!m) {
                return {};
            }
            (*ceto::mad(submatches)).insert((*ceto::mad(m)).begin(), (*ceto::mad(m)).end());
        }
        auto pattern_iterator { (*ceto::mad((*ceto::mad(pattern)).args)).cbegin() } ;
        auto arg_iterator { (*ceto::mad((*ceto::mad(node)).args)).cbegin() } ;
        while (true) {            if (pattern_iterator == (*ceto::mad((*ceto::mad(pattern)).args)).end()) {
                if (arg_iterator != (*ceto::mad((*ceto::mad(node)).args)).end()) {
                    return {};
                } else {
                    break;
                }
            }
            auto subpattern { (*pattern_iterator) } ;
            if ((std::dynamic_pointer_cast<const Identifier>(subpattern) != nullptr)) {
                const auto search = (*ceto::mad(params)).find((*ceto::mad_smartptr((*ceto::mad(subpattern)).name())).value());
                if (search != (*ceto::mad(params)).end()) {
                    const auto param_name = (search -> first);
                    const auto matched_param = (search -> second);
                    if (const auto type_match = std::dynamic_pointer_cast<const TypeOp>(matched_param)) {
                        if (const auto list_param = std::dynamic_pointer_cast<const ListLiteral>((*ceto::mad(type_match)).rhs())) {
                            if ((*ceto::mad((*ceto::mad(list_param)).args)).size() != 1) {
                                throw SemanticAnalysisError{"bad ListLiteral args in macro param"};
                            }
                            const auto wildcard_list_type = ceto::bounds_check((*ceto::mad(list_param)).args, 0);
                            if (!(std::dynamic_pointer_cast<const Identifier>(wildcard_list_type) != nullptr)) {
                                throw SemanticAnalysisError{"bad ListLiteral arg type in macro param"};
                            }
                            const auto wildcard_list_name = (*ceto::mad(type_match)).lhs();
                            if (!(std::dynamic_pointer_cast<const Identifier>(wildcard_list_name) != nullptr)) {
                                throw SemanticAnalysisError{"arg of type ListLiteral must be an identifier"};
                            }
                            const auto wildcard_type_op = std::make_shared<const TypeOp>(":", std::vector<std::shared_ptr<const Node>>{wildcard_list_name, wildcard_list_type});
                            const std::map<std::string,std::shared_ptr<const Node>> wildcard_list_params = {{(*ceto::mad_smartptr((*ceto::mad(wildcard_list_name)).name())).value(), wildcard_type_op}};
                            std::vector<std::shared_ptr<const Node>> wildcard_list_matches = std::vector<std::shared_ptr<const Node>>{}; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::vector<std::shared_ptr<const Node>>{}), std::remove_cvref_t<decltype(wildcard_list_matches)>>);
                            while (arg_iterator != (*ceto::mad((*ceto::mad(node)).args)).end()) {                                const auto arg = (*arg_iterator);
                                if (macro_matches(arg, wildcard_list_name, wildcard_list_params)) {
                                    (*ceto::mad(wildcard_list_matches)).push_back(arg);
                                } else {
                                    break;
                                }
                                arg_iterator += 1;
                            }
                            ceto::bounds_check(submatches, param_name) = std::make_shared<const ListLiteral>(wildcard_list_matches);
                            pattern_iterator += 1;
                            if (pattern_iterator == (*ceto::mad((*ceto::mad(pattern)).args)).end()) {
                                if (arg_iterator != (*ceto::mad((*ceto::mad(node)).args)).end()) {
                                    return {};
                                }
                                break;
                            }
                            subpattern = (*pattern_iterator);
                        }
                    }
                }
            }
            if (arg_iterator == (*ceto::mad((*ceto::mad(node)).args)).end()) {
                if (pattern_iterator != (*ceto::mad((*ceto::mad(pattern)).args)).end()) {
                    return {};
                }
                break;
            }
            const auto arg = (*arg_iterator);
            const auto m = macro_matches(arg, subpattern, params);
            if (!m) {
                return {};
            }
            (*ceto::mad(submatches)).insert((*ceto::mad(m)).begin(), (*ceto::mad(m)).end());
            arg_iterator += 1;
            pattern_iterator += 1;
        }
        return submatches;
    }

    inline auto call_macro_impl(const std::shared_ptr<const MacroDefinition>&  definition,  const std::map<std::string,std::shared_ptr<const Node>> &  match) -> std::variant<std::shared_ptr<const Node>,ceto::macros::Skip> {
        const auto handle = CETO_DLOPEN((*ceto::mad((*ceto::mad(definition)).dll_path)).c_str());
        if (!handle) {
            throw std::runtime_error("Failed to open macro dll: " + (*ceto::mad(definition)).dll_path);
        }
        const auto fptr = CETO_DLSYM(handle, (*ceto::mad((*ceto::mad(definition)).impl_function_name)).c_str());
        if (!fptr) {
            throw std::runtime_error((("Failed to find symbol " + (*ceto::mad(definition)).impl_function_name) + " in dll ") + (*ceto::mad(definition)).dll_path);
        }
        const auto f = reinterpret_cast<decltype(+[]( const std::map<std::string,std::shared_ptr<const Node>> &  m) -> std::shared_ptr<const Node> {
                if constexpr (!std::is_void_v<decltype(nullptr)>&& !std::is_void_v<std::shared_ptr<const Node>>) { return nullptr; } else { static_cast<void>(nullptr); };
                })>(fptr);
        return (*f)(match);
    }

struct MacroDefinitionVisitor : public BaseVisitor<MacroDefinitionVisitor> {

    std::function<void(std::shared_ptr<const MacroDefinition>, const std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> &)> on_visit_definition;

    std::unique_ptr<MacroScope> current_scope = nullptr; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(nullptr), std::remove_cvref_t<decltype(current_scope)>>);

    std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> replacements = {};

    std::map<std::shared_ptr<const Node>,std::vector<std::shared_ptr<const MacroDefinition>>> skipped_definitions = {};

        inline auto expand(const std::shared_ptr<const Node>&  node) -> auto {
            auto const * scope { (&(this -> current_scope)) -> get() } ;
            while (scope) {                for(const auto& definition : ceto::util::reversed(scope -> macro_definitions)) {
                    auto skip_definition { false } ;
                    for(  const auto& [key, defns] : (this -> skipped_definitions)) {
                        if (ceto::util::contains(defns, definition)) {
                            skip_definition = true;
                            break;
                        }
                    }
                    if (skip_definition) {
                        continue;
                    }
                    const auto match = macro_matches(node, (*ceto::mad(definition)).pattern_node, (*ceto::mad(definition)).parameters);
                    if (match) {
                        const auto result_variant = call_macro_impl(definition, (*ceto::mad_smartptr(match)).value());
                        if (std::holds_alternative<ceto::macros::Skip>(result_variant)) {
                            (*ceto::mad(ceto::bounds_check(this -> skipped_definitions, node))).push_back(definition);
                        } else {
                            const auto replacement = std::get<std::shared_ptr<const Node>>(result_variant);
                            if (replacement && (replacement != node)) {
                                ceto::bounds_check(this -> replacements, node) = replacement;
                                (*ceto::mad(replacement)).accept((*this));
                                return true;
                            }
                        }
                    }
                }
                scope = (scope -> parent);
            }
            return false;
        }

        inline auto _cleanup_skipped(const std::shared_ptr<const Node>&  node) -> void {
            const auto it = (*ceto::mad(this -> skipped_definitions)).find(node);
            if (it != (*ceto::mad(this -> skipped_definitions)).end()) {
                (*ceto::mad(it -> second)).clear();
            }
        }

        inline auto visit(const Node&  n) -> void override {
            const auto node = ceto::shared_from((&n));
            if (this -> expand(node)) {
                return;
            }
            if ((*ceto::mad(node)).func) {
                (*ceto::mad((*ceto::mad(node)).func)).accept((*this));
            }
            for(const auto& arg : (*ceto::mad(node)).args) {
                (*ceto::mad(arg)).accept((*this));
            }
            this -> _cleanup_skipped(node);
        }

        inline auto visit(const Call&  call_node) -> void override {
            const auto node = ceto::shared_from((&call_node));
            if (this -> expand(node)) {
                return;
            }
            (*ceto::mad((*ceto::mad(node)).func)).accept((*this));
            for(const auto& arg : (*ceto::mad(node)).args) {
                (*ceto::mad(arg)).accept((*this));
            }
            if ((*ceto::mad((*ceto::mad(node)).func)).name() != "defmacro") {
                return;
            }
            if ((*ceto::mad((*ceto::mad(node)).args)).size() < 2) {
                throw SemanticAnalysisError{"bad defmacro args"};
            }
            const auto pattern = ceto::bounds_check((*ceto::mad(node)).args, 0);
            if (!(std::dynamic_pointer_cast<const Block>((*ceto::mad((*ceto::mad(node)).args)).back()) != nullptr)) {
                throw SemanticAnalysisError{"last defmacro arg must be a Block"};
            }
            auto parameters { std::map<std::string,std::shared_ptr<const Node>>{} } ;
            #if defined(__clang__) && (__clang_major__ < 16)
                const auto match_args = std::vector((*ceto::mad((*ceto::mad(node)).args)).cbegin() + 1, (*ceto::mad((*ceto::mad(node)).args)).cend() - 1);
            #else
                const auto match_args = std::span((*ceto::mad((*ceto::mad(node)).args)).cbegin() + 1, (*ceto::mad((*ceto::mad(node)).args)).cend() - 1);
            #endif

            for(const auto& arg : match_args) {
                const auto name = [&]() {if ((std::dynamic_pointer_cast<const Identifier>(arg) != nullptr)) {
                    return (*ceto::mad_smartptr((*ceto::mad(arg)).name())).value();
                } else if (!(std::dynamic_pointer_cast<const TypeOp>(arg) != nullptr)) {
                    throw SemanticAnalysisError{"bad defmacro param type"};
                } else if (!(std::dynamic_pointer_cast<const Identifier>(ceto::bounds_check((*ceto::mad(arg)).args, 0)) != nullptr)) {
                    throw SemanticAnalysisError{"bad typed defmacro param"};
                } else {
                    return (*ceto::mad_smartptr((*ceto::mad(ceto::bounds_check((*ceto::mad(arg)).args, 0))).name())).value();
                }}()
;
                const auto i = (*ceto::mad(parameters)).find(name);
                if (i != (*ceto::mad(parameters)).end()) {
                    throw SemanticAnalysisError{"duplicate defmacro params"};
                }
                (*ceto::mad_smartptr(parameters)).emplace(name, arg);
            }
            const auto defn = std::make_shared<const MacroDefinition>(node, pattern, parameters);
            (*ceto::mad(this -> current_scope)).add_definition(defn);
            this -> on_visit_definition(defn, this -> replacements);
            this -> _cleanup_skipped(node);
        }

        inline auto visit(const Module&  node) -> void override {
            auto s = std::make_unique<MacroScope>();
            (this -> current_scope) = std::move(s);
            for(const auto& arg : (*ceto::mad(node)).args) {
                (*ceto::mad(arg)).accept((*this));
            }
        }

        inline auto visit(const Block&  block_node) -> void override {
            std::unique_ptr<MacroScope> outer = std::move(this -> current_scope); static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::move(this -> current_scope)), std::remove_cvref_t<decltype(outer)>>);
            (this -> current_scope) = (*ceto::mad(outer)).enter_scope();
            const auto node = ceto::shared_from((&block_node));
            if (this -> expand(node)) {
                return;
            }
            for(const auto& arg : (*ceto::mad(node)).args) {
                (*ceto::mad(arg)).accept((*this));
            }
            (this -> current_scope) = std::move(outer);
            this -> _cleanup_skipped(node);
        }

    explicit MacroDefinitionVisitor(std::function<void(std::shared_ptr<const MacroDefinition>, const std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> &)> on_visit_definition) : on_visit_definition(on_visit_definition) {}

    MacroDefinitionVisitor() = delete;

};

    inline auto expand_macros(const std::shared_ptr<const Module>&  node, const std::function<void(std::shared_ptr<const MacroDefinition>, const std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> &)>  on_visit) -> std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> {
        auto visitor { MacroDefinitionVisitor{on_visit} } ;
        (*ceto::mad(node)).accept(visitor);
        return (*ceto::mad(visitor)).replacements;
    }

