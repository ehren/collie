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
#include <span>
;
#include <functional>
;
#include "ast.donotedit.autogenerated.h"
;
#include "visitor.donotedit.autogenerated.h"
;
struct SemanticAnalysisError : public std::runtime_error {

using std::runtime_error::runtime_error;

};

struct MacroDefinition : public ceto::object {

    std::shared_ptr<const Call> defmacro_node;

    std::shared_ptr<const Node> pattern_node;

    std::map<std::string,std::shared_ptr<const Node>> parameters;

    std::string dll_path = {};

    std::string impl_function_name = {};

    explicit MacroDefinition(std::shared_ptr<const Call> defmacro_node, std::shared_ptr<const Node> pattern_node, std::map<std::string,std::shared_ptr<const Node>> parameters) : defmacro_node(std::move(defmacro_node)), pattern_node(std::move(pattern_node)), parameters(parameters) {}

    MacroDefinition() = delete;

};

struct MacroScope : public ceto::shared_object, public std::enable_shared_from_this<MacroScope> {

    std::weak_ptr<const MacroScope> _parent = {};

    std::vector<MacroDefinition> macro_definitions = std::vector<MacroDefinition>{}; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(std::vector<MacroDefinition>{}), std::remove_cvref_t<decltype(macro_definitions)>>);

        inline auto parent() const -> auto {
            return ceto::mado(this -> _parent)->lock();
        }

        inline auto add_definition(const MacroDefinition&  defn) -> void {
            ceto::mado(this -> macro_definitions)->push_back(defn);
        }

        inline auto enter_scope() const -> std::shared_ptr<MacroScope> {
            const auto self = ceto::shared_from(this);
            auto m { std::make_shared<decltype(MacroScope())>() } ;
            ceto::mado(m)->_parent = self;
            return m;
        }

};

struct MacroDefinitionVisitor : public BaseVisitor<MacroDefinitionVisitor> {

    std::function<void(MacroDefinition)> on_visit_definition;

    std::unordered_map<std::shared_ptr<const Node>,std::shared_ptr<const MacroScope>> macro_scopes = {};

    std::shared_ptr<MacroScope> current_scope = nullptr; static_assert(ceto::is_non_aggregate_init_and_if_convertible_then_non_narrowing_v<decltype(nullptr), std::remove_cvref_t<decltype(current_scope)>>);

        inline auto visit(const Node&  node) -> void override {
            ceto::maybe_bounds_check_access(this -> macro_scopes,ceto::shared_from((&node))) = (this -> current_scope);
if (ceto::mado(node)->func) {
                ceto::mado(ceto::mado(node)->func)->accept((*this));
            }
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
        }

        inline auto visit(const Call&  node) -> void override {
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
            auto parameters { std::map<std::string,std::shared_ptr<const Node>>{} } ;
            const auto rest = std::span(ceto::mado(ceto::mado(node)->args)->cbegin() + 2, ceto::mado(ceto::mado(node)->args)->cend());
            for(const auto& arg : rest) {
                const auto name = [&]() {if ((std::dynamic_pointer_cast<const Identifier>(arg) != nullptr)) {
                    return ceto::mad(ceto::mado(arg)->name())->value();
                } else {
if (!(std::dynamic_pointer_cast<const TypeOp>(arg) != nullptr)) {
                        throw SemanticAnalysisError{"bad defmacro param type"};
                    }
if (!(std::dynamic_pointer_cast<const Identifier>(ceto::maybe_bounds_check_access(ceto::mado(arg)->args,0)) != nullptr)) {
                        throw SemanticAnalysisError{"bad typed defmacro param"};
                    }
                    return ceto::mad(ceto::mado(ceto::maybe_bounds_check_access(ceto::mado(arg)->args,0))->name())->value();
                }}()
;
                const auto i = ceto::mado(parameters)->find(name);
if (i != ceto::mado(parameters)->end()) {
                    throw SemanticAnalysisError{"duplicate defmacro params"};
                }
                ceto::mad(parameters)->emplace(name, arg);
            }
            const auto defn = MacroDefinition{ceto::shared_from((&node)), pattern, parameters};
            ceto::mado(this -> current_scope)->add_definition(defn);
            this -> on_visit_definition(defn);
        }

        inline auto visit(const Module&  node) -> void override {
            auto s { std::make_shared<decltype(MacroScope())>() } ;
            ceto::maybe_bounds_check_access(this -> macro_scopes,ceto::shared_from((&node))) = s;
            (this -> current_scope) = s;
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
        }

        inline auto visit(const Block&  node) -> void override {
            const auto outer = (this -> current_scope);
            const auto inner = ceto::mado(outer)->enter_scope();
            ceto::maybe_bounds_check_access(this -> macro_scopes,ceto::shared_from((&node))) = outer;
            (this -> current_scope) = inner;
            for(const auto& arg : ceto::mado(node)->args) {
                ceto::mado(arg)->accept((*this));
            }
            (this -> current_scope) = outer;
        }

    explicit MacroDefinitionVisitor(std::function<void(MacroDefinition)> on_visit_definition) : on_visit_definition(on_visit_definition) {}

    MacroDefinitionVisitor() = delete;

};

    inline auto visit_macro_definitions(const std::shared_ptr<const Module>&  node, const std::function<void(MacroDefinition)>  on_visit) -> auto {
        auto visitor { MacroDefinitionVisitor{on_visit} } ;
        ceto::mado(node)->accept(visitor);
        return ceto::mado(visitor)->macro_scopes;
    }

