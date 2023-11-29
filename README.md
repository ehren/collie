# ceto

**ceto** is an experimental "pythonish" dialect of C++:

```python
include <numeric>
include <thread>
include <ranges>
include <iostream>


class (Foo:
    data_member

    def (method, param:
        std.cout << "size: " << param.size()  << "\n"
        return self
    )

    def (size:
        return self.data_member.size()
    )
)


def (string_join, vec: [std.string], sep = ", "s:

    if (vec.empty():
        return ""
    )

    return std.accumulate(vec.cbegin() + 1, vec.cend(), vec[0],
        lambda[&sep] (a, b, a + sep + b))

): std.string


defmacro(s.join(v), s: StringLiteral, v:
    return quote(string_join(unquote(v), unquote(s)))
)


struct (Oops(std.runtime_error):
    pass
)


def (main, argc: int, argv: const:char:ptr:const:ptr:
    args : mut = []

    for (a in std.span(argv, argc):
        args.append(std.string(a))
    )

    more = if (argc == 0:
        "no args"s
    elif argc > 15:
        throw (Oops("too many args entirely:" + ",".join(args))
    else:
        "end"s
    )
    args.append(more)

    summary = ", ".join(args)

    f = Foo(summary)
    f.method(args)
    f.method(f)    

    t: mut = std.thread(lambda(:
        d = f.method(f).data_member 
        std.cout << if (d.size() < 100: d else: "too much data!") << std.endl
    ): void)

    t.join()
)
```

```
$ ceto kitchensink.ctp a b c d e f
size: 8
size: 33
./kitchensink, a, b, c, d, e, f, end
```

## More Examples:

Classes definitions are intended to resemble python dataclasses

```python
class (Generic:
    x  # implicit 1-arg constructor, deleted 0-arg constructor
)

class (Concrete(Generic):
    def (init, x: int:
        super.init(x)
    )
)

class (Generic2(Generic):
    y
    def (init, x, y:
        self.y = y
        super.init(x)
    )
)

def (main:
    f = Generic("5")
    f2 = Concrete(5)
    #f2e = Concrete("5")  # error
    f3 = Generic2([5], 5.0)
    std.cout << f.x << f2.x << f3.x[0]
)

# Output: 555.0

```

You can code a simple visitor pattern almost like\* Java

```python
class (Node)
class (Identifier)
class (BinOp)
class (Add)

class (Visitor:

    def (visit: virtual:mut, node: Node): void = 0

    def (visit: virtual:mut, node: Identifier): void = 0

    def (visit: virtual:mut, node: BinOp): void = 0

    def (visit: virtual:mut, node: Add): void = 0
)

class (Node:
    loc : int

    def (accept: virtual, visitor: Visitor:mut:
        visitor.visit(self)
    )
)

class (Identifier(Node):
    name : std.string

    def (init, name, loc=0:
        # a user defined constructor is present - 1-arg constructor of Node is not inherited
        self.name = name  # implicitly occurs in initializer list
        super.init(loc)   # same
    )

    def (accept: override, visitor: Visitor:mut:
        visitor.visit(self)
    )
)

class (BinOp(Node):
    args : [Node]

    def (init, args, loc=0:
        self.args = args
        super.init(loc)
    )

    def (accept: override, visitor: Visitor:mut:
        visitor.visit(self)
    )
)

class (Add(BinOp):
    # inherits 2-arg constructor from BinOp (because no user defined init is present)

    def (accept: override, visitor: Visitor:mut:
        visitor.visit(self)
    )
)

class (SimpleVisitor(Visitor):
    record = s""

    def (visit: override:mut, node: Node:
        self.record += "visiting Node\n"
    )

    def (visit: override:mut, ident: Identifier:
        self.record += "visiting Identifier " + ident.name + "\n"
    )

    def (visit: override:mut, node: BinOp:
        self.record += "visiting BinOp\n"

        for (arg in node.args:
            arg.accept(self)
        )
    )

    def (visit: override:mut, node: Add:
        self.record += "visiting Add\n"

        for (arg in node.args:
            arg.accept(self)
        )
    )
)

def (main:
    node = Node(0)
    ident = Identifier("a", 5)
    args: [Node] = [ident, node, ident]
    add: Add = Add(args)

    simple_visitor: mut = SimpleVisitor()
    ident.accept(simple_visitor)
    add.accept(simple_visitor)

    std.cout << simple_visitor.record
)

# Output:
# visiting Identifier a
# visiting Add
# visiting Identifier a
# visiting Node
# visiting Identifier a

```
