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

//#include <concepts>
//#include <ranges>
//#include <numeric>


#include "ceto.h"


#include <unordered_map>
;
#include <ranges>
;
#include <functional>
;
#include <span>
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

    std::shared_ptr<const Call> defmacro_node;

    std::shared_ptr<const Node> pattern_node;

    std::shared_ptr<const Block> body;

    std::map<std::string,std::shared_ptr<const Node>> parameters;

    std::string dll_path = {};

    std::string impl_function_name = {};

    explicit MacroDefinition(std::shared_ptr<const Call> defmacro_node, std::shared_ptr<const Node> pattern_node, std::shared_ptr<const Block> body, std::map<std::string,std::shared_ptr<const Node>> parameters) : defmacro_node(std::move(defmacro_node)), pattern_node(std::move(pattern_node)), body(std::move(body)), parameters(parameters) {}

    MacroDefinition() = delete;

};

struct MacroScope : public ceto::object {

    MacroScope const * parent = nullptr; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(nullptr), std::remove_cvref_t<decltype(parent)>>);

    std::vector<std::shared_ptr<const MacroDefinition>> macro_definitions = std::vector<std::shared_ptr<const MacroDefinition>>{}; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::vector<std::shared_ptr<const MacroDefinition>>{}), std::remove_cvref_t<decltype(macro_definitions)>>);

        inline auto add_definition(const std::shared_ptr<const MacroDefinition>&  defn) -> void {
            ceto::mado(this -> macro_definitions)->push_back(defn);
        }

        inline auto enter_scope() const -> std::unique_ptr<MacroScope> {
            auto s { std::make_unique<decltype(MacroScope())>() } ;
            ceto::mado(s)->parent = this;
            return s;
        }

};

    inline auto macro_matches(const std::shared_ptr<const Node>&  node, const std::shared_ptr<const Node>&  pattern,  const std::map<std::string,std::shared_ptr<const Node>> &  params) -> std::optional<std::map<std::string,std::shared_ptr<const Node>>> {
        ((((std::cout << "node: ") << ceto::mado(node)->repr()) << " pattern: ") << ceto::mado(pattern)->repr()) << "\n";
        if ((std::dynamic_pointer_cast<const Identifier>(pattern) != nullptr)) {
            const auto search = ceto::mado(params)->find(ceto::mad(ceto::mado(pattern)->name())->value());
            if (search != ceto::mado(params)->end()) {
                const auto param_name = (search -> first);
                const auto matched_param = (search -> second);
                if ((std::dynamic_pointer_cast<const Identifier>(matched_param) != nullptr)) {
                    return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                } else if (const auto typeop = std::dynamic_pointer_cast<const TypeOp>(matched_param)) {
                    const auto ast_name = ceto::mado(typeop)->rhs();
                    if ((std::dynamic_pointer_cast<const Identifier>(ast_name) != nullptr)) {
                        if ((ceto::mado(ast_name)->name() == "BinOp") && (std::dynamic_pointer_cast<const BinOp>(node) != nullptr)) {
                            return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                        } else if (((ceto::mado(ast_name)->name() == "UnOp") && (std::dynamic_pointer_cast<const UnOp>(node) != nullptr))) {
                            return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                        }
                        const auto node_class_name = ceto::mado(node)->classname();
                        if (node_class_name == ceto::mad(ceto::mado(ceto::mado(typeop)->rhs())->name())->value()) {
                            return std::map<std::string,std::shared_ptr<const Node>>{{param_name, node}};
                        }
                    }
                }
            }
        }
        if (typeid((*node)) != typeid((*pattern))) {
            return {};
        }
        if (ceto::mado(ceto::mado(node)->args)->size() != ceto::mado(ceto::mado(pattern)->args)->size()) {
            return {};
        }
        if ((ceto::mado(node)->func == nullptr) != (ceto::mado(pattern)->func == nullptr)) {
            return {};
        }
        if (((ceto::mado(ceto::mado(node)->args)->size() == 0) && (ceto::mado(node)->func == nullptr)) && (ceto::mado(pattern)->func == nullptr)) {
            if (ceto::mado(node)->repr() == ceto::mado(pattern)->repr()) {
                return std::map<std::string,std::shared_ptr<const Node>>{};
            }
            return {};
        }
        auto submatches { std::map<std::string,std::shared_ptr<const Node>>{} } ;
        for(const auto& i : range(ceto::mado(ceto::mado(node)->args)->size())) {
            const auto m = macro_matches(ceto::maybe_bounds_check_access(ceto::mado(node)->args,i), ceto::maybe_bounds_check_access(ceto::mado(pattern)->args,i), params);
            if (!m) {
                return {};
            }
            ceto::mado(submatches)->insert(ceto::mado(m)->begin(), ceto::mado(m)->end());
        }
        if (ceto::mado(node)->func) {
            const auto m = macro_matches(ceto::mado(node)->func, ceto::mado(pattern)->func, params);
            if (!m) {
                return {};
            }
            ceto::mado(submatches)->insert(ceto::mado(m)->begin(), ceto::mado(m)->end());
        }
        return submatches;
    }

    inline auto call_macro_impl(const std::shared_ptr<const MacroDefinition>&  definition, const std::map<std::string,std::shared_ptr<const Node>>  match) -> std::shared_ptr<const Node> {
        const auto handle = CETO_DLOPEN(ceto::mado(ceto::mado(definition)->dll_path)->c_str());
        if (!handle) {
            throw std::runtime_error("Failed to open macro dll: " + ceto::mado(definition)->dll_path);
        }
        const auto fptr = CETO_DLSYM(handle, ceto::mado(ceto::mado(definition)->impl_function_name)->c_str());
        if (!fptr) {
            throw std::runtime_error((("Failed to find symbol " + ceto::mado(definition)->impl_function_name) + " in dll ") + ceto::mado(definition)->dll_path);
        }
        const auto f = reinterpret_cast<decltype(+[](const std::map<std::string,std::shared_ptr<const Node>>  m) -> std::shared_ptr<const Node> {
                if constexpr (!std::is_void_v<decltype(nullptr)>&& !std::is_void_v<std::shared_ptr<const Node>>) { return nullptr; } else { static_cast<void>(nullptr); };
                })>(fptr);
        return (*f)(match);
    }

struct MacroDefinitionVisitor : public BaseVisitor<MacroDefinitionVisitor> {

    std::function<void(std::shared_ptr<const MacroDefinition>)> on_visit_definition;

    std::unique_ptr<MacroScope> current_scope = nullptr; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(nullptr), std::remove_cvref_t<decltype(current_scope)>>);

    std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> replacements = {};

        inline auto expand(const std::shared_ptr<const Node>&  node) -> void {
            auto const * scope { (&(this -> current_scope)) -> get() } ;
            while (scope) {                for(const auto& definition : reversed(scope -> macro_definitions)) {
                    const auto match = macro_matches(node, ceto::mado(definition)->pattern_node, ceto::mado(definition)->parameters);
                    if (match) {
                        const auto replacement = call_macro_impl(definition, ceto::mad(match)->value());
                        if (replacement) {
                            ceto::maybe_bounds_check_access(this -> replacements,node) = replacement;
                            return;
                        }
                    }
                }
                scope = (scope -> parent);
            }
        }

        inline auto visit(const Node&  node) -> void override {
            this -> expand(ceto::shared_from((&node)));
            if (ceto::mado(node)->func) {
                ceto::mado(ceto::mado(node)->func)->accept((*this));
            }
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
        }

        inline auto visit(const Call&  node) -> void override {
            this -> expand(ceto::shared_from((&node)));
            if (ceto::mado(ceto::mado(node)->func)->name() != "defmacro") {
                ceto::mado(ceto::mado(node)->func)->accept((*this));
                for(const auto& arg : ceto::mado(node)->args) {
                    ceto::mado(arg)->accept((*this));
                }
                return;
            }
            if (ceto::mado(ceto::mado(node)->args)->size() < 2) {
                throw SemanticAnalysisError{"bad defmacro args"};
            }
            const auto pattern = ceto::maybe_bounds_check_access(ceto::mado(node)->args,0);
            const auto body = std::dynamic_pointer_cast<const Block>(ceto::mado(ceto::mado(node)->args)->back());
            if (!body) {
                throw SemanticAnalysisError{"last defmacro arg must be a Block"};
            }
            auto parameters { std::map<std::string,std::shared_ptr<const Node>>{} } ;
            #if defined(__clang__) && (__clang_major__ < 16)
                const auto match_args = std::vector(ceto::mado(ceto::mado(node)->args)->cbegin() + 1, ceto::mado(ceto::mado(node)->args)->cend() - 1);
            #else
                const auto match_args = std::span(ceto::mado(ceto::mado(node)->args)->cbegin() + 1, ceto::mado(ceto::mado(node)->args)->cend() - 1);
            #endif

            for(const auto& arg : match_args) {
                const auto name = [&]() {if ((std::dynamic_pointer_cast<const Identifier>(arg) != nullptr)) {
                    return ceto::mad(ceto::mado(arg)->name())->value();
                } else if (!(std::dynamic_pointer_cast<const TypeOp>(arg) != nullptr)) {
                    throw SemanticAnalysisError{"bad defmacro param type"};
                } else if (!(std::dynamic_pointer_cast<const Identifier>(ceto::maybe_bounds_check_access(ceto::mado(arg)->args,0)) != nullptr)) {
                    throw SemanticAnalysisError{"bad typed defmacro param"};
                } else {
                    return ceto::mad(ceto::mado(ceto::maybe_bounds_check_access(ceto::mado(arg)->args,0))->name())->value();
                }}()
;
                const auto i = ceto::mado(parameters)->find(name);
                if (i != ceto::mado(parameters)->end()) {
                    throw SemanticAnalysisError{"duplicate defmacro params"};
                }
                ceto::mad(parameters)->emplace(name, arg);
            }
            const auto defn = std::make_shared<const decltype(MacroDefinition{ceto::shared_from((&node)), pattern, body, parameters})>(ceto::shared_from((&node)), pattern, body, parameters);
            ceto::mado(this -> current_scope)->add_definition(defn);
            this -> on_visit_definition(defn);
        }

        inline auto visit(const Module&  node) -> void override {
            auto s { std::make_unique<decltype(MacroScope())>() } ;
            (this -> current_scope) = std::move(s);
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
        }

        inline auto visit(const Block&  node) -> void override {
            std::unique_ptr<MacroScope> outer = std::move(this -> current_scope); static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::move(this -> current_scope)), std::remove_cvref_t<decltype(outer)>>);
            (this -> current_scope) = ceto::mado(outer)->enter_scope();
            this -> expand(ceto::shared_from((&node)));
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
            (this -> current_scope) = std::move(outer);
        }

    explicit MacroDefinitionVisitor(std::function<void(std::shared_ptr<const MacroDefinition>)> on_visit_definition) : on_visit_definition(on_visit_definition) {}

    MacroDefinitionVisitor() = delete;

};

    inline auto expand_macros(const std::shared_ptr<const Module>&  node, const std::function<void(std::shared_ptr<const MacroDefinition>)>  on_visit) -> std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const Node>> {
        auto visitor { MacroDefinitionVisitor{on_visit} } ;
        ceto::mado(node)->accept(visitor);
        return ceto::mado(visitor)->replacements;
    }
