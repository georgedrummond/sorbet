#ifndef SRUBY_TREES_H
#define SRUBY_TREES_H

#include "../common/common.h"
#include "Context.h"
#include "Symbols.h"
#include <memory>
#include <vector>

namespace ruby_typer {
namespace ast {

class Statement {
public:
    Statement() = default;
    virtual ~Statement() = default;
    virtual std::string toString(ContextBase &ctx, int tabs = 0) = 0;
};

class Expression : public Statement {};

class ControlFlow : public Expression {};

class Declaration : public Expression {
public:
    SymbolRef symbol;

    Declaration(SymbolRef symbol);
};

class ClassDef : public Declaration {
public:
    inline SymbolRef parent(Context ctx) {
        return symbol.info(ctx).parent(ctx);
    }

    inline std::vector<SymbolRef> &mixins(Context ctx) {
        return symbol.info(ctx).mixins(ctx);
    }

    std::vector<std::unique_ptr<Statement>> rhs;
    std::unique_ptr<Expression> name;
    bool isModule;

    ClassDef(SymbolRef symbol, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Statement>> &rhs,
             bool isModule);

    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class MethodDef : public Declaration {
public:
    std::unique_ptr<Expression> rhs;
    std::vector<std::unique_ptr<Expression>> args;
    NameRef name;
    bool isSelf;

    MethodDef(SymbolRef symbol, NameRef name, std::vector<std::unique_ptr<Expression>> &args,
              std::unique_ptr<Expression> rhs, bool isSelf);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class ConstDef : public Declaration {
public:
    std::unique_ptr<Expression> rhs;

    ConstDef(SymbolRef symbol, std::unique_ptr<Expression> rhs);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class If : public ControlFlow {
public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Expression> thenp;
    std::unique_ptr<Expression> elsep;
    If(std::unique_ptr<Expression> cond, std::unique_ptr<Expression> thenp, std::unique_ptr<Expression> elsep);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Breakable : public ControlFlow {
public:
    u1 break_tag;

    Breakable(u1 break_tag);
};

class While : public Breakable {
public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Statement> body;

    While(u1 break_tag, std::unique_ptr<Expression> cond, std::unique_ptr<Statement> body);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class For : public Breakable {
    // TODO
};

class Break : public ControlFlow {
public:
    u1 break_tag;

    Break(u1 break_tag);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Next : public ControlFlow {
public:
    u1 break_tag;

    Next(u1 break_tag);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Return : public ControlFlow {
public:
    std::unique_ptr<Expression> expr;

    Return(std::unique_ptr<Expression> expr);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Rescue : public ControlFlow {
public:
    std::unique_ptr<Expression> body;
    SymbolRef binder;
    SymbolRef binder_type;
    std::unique_ptr<Expression> handler;

    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Ident : public Expression {
public:
    SymbolRef symbol;
    NameRef name;

    Ident(SymbolRef symbol);
    Ident(NameRef name, SymbolRef symbol);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Symbol : public Expression {
public:
    NameRef name;

    Symbol(NameRef name);

    virtual std::string toString(ContextBase &ctx, int tabs);
};

class Assign : public Expression {
public:
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;

    Assign(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Send : public Expression {
public:
    std::unique_ptr<Expression> recv;
    NameRef fun;
    std::vector<std::unique_ptr<Expression>> args;

    Send(std::unique_ptr<Expression> recv, NameRef fun, std::vector<std::unique_ptr<Expression>> &&args);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class New : public Expression {
public:
    SymbolRef claz;
    std::vector<std::unique_ptr<Expression>> args;

    New(SymbolRef claz, std::vector<std::unique_ptr<Expression>> &&args);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Super : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> args;

    Super(std::vector<std::unique_ptr<Expression>> &&args);

    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class NamedArg : public Expression {
public:
    NameRef name;
    std::unique_ptr<Expression> arg;

    NamedArg(NameRef name, std::unique_ptr<Expression> arg);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Hash : public Expression {
    // TODO
};

class Array : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> elems;

    Array(std::vector<std::unique_ptr<Expression>> &elems);

    virtual std::string toString(ContextBase &ctx, int tabs);
};

class FloatLit : public Expression {
public:
    float value;

    FloatLit(float value);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class IntLit : public Expression {
public:
    int value;

    IntLit(int value);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class StringLit : public Expression {
public:
    NameRef value;

    StringLit(NameRef value);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class ConstantLit : public Expression {
public:
    NameRef cnst;
    std::unique_ptr<Expression> scope;

    ConstantLit(std::unique_ptr<Expression> scope, NameRef cnst);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class ArraySplat : public Expression {
public:
    std::unique_ptr<Expression> arg;

    ArraySplat(std::unique_ptr<Expression> arg);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class HashSplat : public Expression {
public:
    std::unique_ptr<Expression> arg;

    HashSplat(std::unique_ptr<Expression> arg);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Self : public Expression {
public:
    SymbolRef claz;

    Self(SymbolRef claz);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class Closure : public Expression {
public:
    SymbolRef method;

    Closure(SymbolRef method);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class InsSeq : public Expression {
public:
    std::vector<std::unique_ptr<Statement>> stats;
    std::unique_ptr<Expression> expr;

    InsSeq(std::vector<std::unique_ptr<Statement>> &&stats, std::unique_ptr<Expression> expr);
    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class EmptyTree : public Expression {

    virtual std::string toString(ContextBase &ctx, int tabs = 0);
};

class NotSupported : public Expression {
    std::string why;

public:
    NotSupported(const std::string &why);

    virtual std::string toString(ContextBase &ctx, int tabs);
};

/** https://git.corp.stripe.com/gist/nelhage/51564501674174da24822e60ad770f64
 *
 *  [] - prototype only
 *
 *                 / Control Flow <- while, if, for, break, next, return, rescue, case
 * Pre-CFG-Node <-
 *                 \ Instruction <- assign, send, [new], ident, named_arg, hash, array, literals(symbols, ints, floats,
 * strings, constants, nil), constants(resolver will desugar it into literals), array_splat(*), hash_splat(**), self,
 * insseq, closure)
 *
 *                  \ Definition  <-  class(name, parent, mixins, body)
 *                                    module
 *                                    def
 *                                    defself
 *                                    const_assign
 *
 *
 *
 * know id for: top, bottom, kernel?, basicobject, class, module [postponed], unit, Hash, Array, String, Symbol, float,
 * int, numeric, double, unknown
 *
 *
 *
 * Desugar string concatenation into series of .to_s calls and string concatenations
 */

} // namespace ast
} // namespace ruby_typer

#endif // SRUBY_TREES_H
