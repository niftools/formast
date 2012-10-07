# simple runtime class example

# note: to keep things simple, unused arithmetic operators
# for the format description are not implemented
# a realistic example would implement all operators

import formast

def api_name(name):
    return "_".join(part.lower() for part in name.split())

class RuntimeExprEval(formast.Visitor):
    """Generate code for expression."""
    stack = []

    def __init__(self, obj):
        formast.Visitor.__init__(self)
        self.obj = obj

    def expr_uint(self, v):
        self.stack.append(v)

    def expr_id(self, i):
        self.stack.append(getattr(self, api_name(i)))

    def expr_logical_and(self, e1, e2):
        self.expr(e1)
        v1 = self.stack.pop()
        self.expr(e2)
        v2 = self.stack.pop()
        self.stack.append(v1 and v2)

    def expr_compare_le(self, e1, e2):
        self.expr(e1)
        v1 = self.stack.pop()
        self.expr(e2)
        v2 = self.stack.pop()
        self.stack.append(v1 <= v2)

    def expr_compare_ge(self, e1, e2):
        self.expr(e1)
        v1 = self.stack.pop()
        self.expr(e2)
        v2 = self.stack.pop()
        self.stack.append(v1 >= v2)

class RuntimeClass(object):
    """Base class for runtime class system."""
    _ast_class = None #: AST class node

    def __init__(self):
        RuntimeClassInit(self)

    def read(self):
        RuntimeClassRead(self)

class RuntimeClassInit(formast.Visitor):
    """Construct an instance by visitation of the class AST."""

    def __init__(self, obj):
        formast.Visitor.__init__(self)
        self.obj = obj
        if obj._ast_class.stats.is_initialized():
            self.stats(obj._ast_class.stats.get())

    def stats_attr(self, a):
        # note: in the example, type is always int, so this is rather simple
        if not a.arr1.is_initialized():
            setattr(self.obj, api_name(a.name), 0)
        else:
            setattr(self.obj, api_name(a.name), [])

    def stats_if(self, if_):
        self.stats(if_.then)
        # if_.else_ not used

class RuntimeClassRead(formast.Visitor):
    """Read an instance by visitation of the class AST."""

    def __init__(self, obj):
        formast.Visitor.__init__(self)
        self.obj = obj
        if obj._ast_class.stats.is_initialized():
            self.stats(obj._ast_class.stats.get())

    def stats_attr(self, a):
        # everything is an integer, so this is rather simple
        if not a.arr1.is_initialized():
            setattr(self.obj, api_name(a.name), read_int(stream))
        else:
            expr_eval = RuntimeExprEval()
            expr_eval.expr(a.arr1.get())
            setattr(self.obj, api_name(a.name),
                    [read_int(stream) for i in range(expr_eval.stack.pop())])

    def stats_if(self, if_):
        expr_eval = RuntimeExprEval()
        expr_eval.expr(if_.expr)
        if expr_eval.stack.pop():
            self.stats(if_.then)
        # if_.else_ not used

class RuntimeModule(formast.Visitor):
    """Generate module-like object."""

    def top_class(self, c):
        setattr(self, api_name(c.name),
                type(c.name, (RuntimeClass,), dict(_ast_class=c)))

if __name__ == "__main__":
    top = formast.Top()
    with open("../codegen/integers.xml", "rb") as stream:
        formast.XmlParser().parse_string(stream.read(), top)
    mod = RuntimeModule()
    mod.top(top)
