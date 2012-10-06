# simple code generation example

# note: to keep things simple, unused arithmetic operators
# for the format description are not implemented
# a realistic example would implement all operators

import formast

def api_name(name):
    return "_".join(part.lower() for part in name.split())

class CodeGenIndent:
    """Implements indented printing."""

    def __init__(self, parent=None):
        if parent is None:
            self.indent = 0
            self.lines = []
        else:
            self.indent = parent.indent
            self.lines = parent.lines

    def print_(self, txt=None):
        if txt is not None:
            self.lines.append("    " * self.indent + txt)
        else:
            self.lines.append("")

    def __str__(self):
        return "\n".join(self.lines)

class CodeGenExprEval(formast.Visitor):
    """Generate code for expression."""
    stack = []

    def expr_uint(self, v):
        self.stack.append(str(v))

    def expr_id(self, i):
        self.stack.append("self.%s" % api_name(i))

    def expr_logical_and(self, e1, e2):
        self.expr(e2)
        self.expr(e1)
        self.stack.append("(%s and %s)" % (self.stack.pop(), self.stack.pop()))

    def expr_compare_le(self, e1, e2):
        self.expr(e2)
        self.expr(e1)
        self.stack.append("(%s <= %s)" % (self.stack.pop(), self.stack.pop()))

    def expr_compare_ge(self, e1, e2):
        self.expr(e2)
        self.expr(e1)
        self.stack.append("(%s >= %s)" % (self.stack.pop(), self.stack.pop()))

class CodeGenClassInit(formast.Visitor, CodeGenIndent):
    """Generate __init__ body."""

    def __init__(self, parent=None):
        formast.Visitor.__init__(self)
        CodeGenIndent.__init__(self, parent=parent)

    def stats_attr(self, a):
        if not a.arr1.is_initialized():
            self.print_("self.%s = %s()" % (api_name(a.name), a.class_name))
        else:
            self.print_("self.%s = []"  % api_name(a.name))

    def stats_if(self, if_):
        self.stats(if_.then)
        if if_.else_.is_initialized():
            self.stats(if_.else_.get())

class CodeGenClassRead(formast.Visitor, CodeGenIndent):
    """Generate __init__ body."""

    def __init__(self, parent=None):
        formast.Visitor.__init__(self)
        CodeGenIndent.__init__(self, parent=parent)

    def stats_attr(self, a):
        # everything is an integer, so this is rather simple
        if not a.arr1.is_initialized():
            self.print_("self.%s = struct.unpack('<i', stream.read(4))" % api_name(a.name))
        else:
            expr_eval = CodeGenExprEval()
            expr_eval.expr(a.arr1.get())
            self.print_("self.%s = [" % api_name(a.name))
            self.indent += 1
            self.print_("struct.unpack('<i', stream.read(4))")
            self.print_("for i in range(%s)]" % expr_eval.stack.pop())
            self.indent -= 1
        # arr2 not used

    def stats_if(self, if_):
        expr_eval = CodeGenExprEval()
        expr_eval.expr(if_.expr)
        self.print_("if %s:" % expr_eval.stack.pop())
        self.indent += 1
        self.stats(if_.then)
        self.indent -= 1
        # else not used

class CodeGenModule(CodeGenIndent, formast.Visitor):
    """Generate module."""

    def __init__(self, parent=None):
        formast.Visitor.__init__(self)
        CodeGenIndent.__init__(self, parent=parent)

    def top_class(self, c):
        self.print_("class %s:" % c.name)
        self.indent += 1
        if c.stats.is_initialized():
            self.print_()
            self.print_("def __init__(self):")
            self.indent += 1
            codegeninit = CodeGenClassInit(parent=self)
            codegeninit.stats(c.stats.get())
            self.indent -= 1
            self.print_()
            self.print_("def write(self, stream):")
            self.indent += 1
            codegenread = CodeGenClassRead(parent=self)
            codegenread.stats(c.stats.get())
            self.indent -= 1
        else:
            self.print_("pass")
        self.indent -= 1

if __name__ == "__main__":
    top = formast.Top()
    with open("integers.xml", "rb") as stream:
        formast.XmlParser().parse_string(stream.read(), top)
    codegen = CodeGenModule()
    codegen.top(top)
    print(codegen)
