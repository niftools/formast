# simple runtime class example

# note: to keep things simple, unused arithmetic operators
# for the format description are not implemented
# a realistic example would implement all operators

import formast
import struct

def api_name(name):
    return "_".join(part.lower() for part in name.split())

def read_int(stream):
    return struct.unpack('<i', stream.read(4))[0]

class RuntimeExprEval(formast.Visitor):
    """Evaluate expression at runtime."""
    stack = []

    def __init__(self, obj):
        formast.Visitor.__init__(self)
        self.obj = obj

    def expr_uint(self, v):
        self.stack.append(v)

    def expr_id(self, i):
        self.stack.append(getattr(self.obj, api_name(i)))

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
    """Base class for classes that are created at runtime."""
    _ast_class = None #: AST class node

    def __init__(self):
        RuntimeClassInit(self)

    def read(self, stream):
        RuntimeClassRead(self, stream)

class RuntimeClassInit(formast.Visitor):
    """Initialize an instance of a class."""

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
    """Read an instance of a class."""

    def __init__(self, obj, stream):
        formast.Visitor.__init__(self)
        self.obj = obj
        self.stream = stream
        if obj._ast_class.stats.is_initialized():
            self.stats(obj._ast_class.stats.get())

    def stats_attr(self, a):
        # everything is an integer, so this is rather simple
        if not a.arr1.is_initialized():
            setattr(self.obj, api_name(a.name), read_int(self.stream))
        else:
            expr_eval = RuntimeExprEval(self.obj)
            expr_eval.expr(a.arr1.get())
            setattr(self.obj, api_name(a.name),
                    [read_int(self.stream)
                     for i in range(expr_eval.stack.pop())])

    def stats_if(self, if_):
        expr_eval = RuntimeExprEval(self.obj)
        expr_eval.expr(if_.expr)
        if expr_eval.stack.pop():
            self.stats(if_.then)
        # if_.else_ not used

class RuntimeModule(object):
    """Module-like object that is populated with classes at runtime."""

    def __init__(self, top):
        self._ast_top = top
        RuntimeModuleInit(self)

class RuntimeModuleInit(formast.Visitor):
    """Populate module with classes."""

    def __init__(self, mod):
        formast.Visitor.__init__(self)
        self.mod = mod
        self.top(mod._ast_top)

    def top_class(self, c):
        # create class at runtime
        # see http://docs.python.org/library/functions.html#type
        setattr(self.mod, c.name,
                type(c.name, (RuntimeClass,), dict(_ast_class=c)))

if __name__ == "__main__":
    top = formast.Module()
    with open("../codegen/integers.xml", "rb") as stream:
        formast.XmlParser().parse_string(stream.read(), top)
    mod = RuntimeModule(top)
