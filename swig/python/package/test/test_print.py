"""Test parser with a printer-like visitor."""

import nose.tools
import formast

class Printer(formast.Visitor):

    def __init__(self):
        formast.Visitor.__init__(self)
        self.level = 0
        self.lines = []

    def __str__(self):
        return "\n".join(self.lines)

    def print_(self, txt):
        self.lines.append(" " * self.level + txt)

    def expr_uint(self, v):
        self.print_("uint: %s" % v)

    def expr_add(self, left, right):
        self.print_("add:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_sub(self, left, right):
        self.print_("sub:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_mul(self, left, right):
        self.print_("mul:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_div(self, left, right):
        self.print_("div:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_neg(self, right):
        self.print_("neg:")
        self.level += 1
        self.expr(right)
        self.level -= 1

    def expr_pos(self, right):
        self.print_("pos:")
        self.level += 1
        self.expr(right)
        self.level -= 1

class TestPrint:
    def setup(self):
        self.parser = formast.XmlParser()
        self.printer = Printer()

    def check(self, inp , out):
        expr = self.parser.parse_string(inp)
        self.printer.expr(expr)
        nose.tools.assert_equal(str(self.printer), out)

    def test_uint(self):
        self.check("99", "uint: 99")
    
    def test_add(self):
        self.check("1+2", """\
add:
 uint: 1
 uint: 2""")

    def test_complicated(self):
        self.check("1+(2*3+4)*6/(3-4)", """\
add:
 uint: 1
 div:
  mul:
   add:
    mul:
     uint: 2
     uint: 3
    uint: 4
   uint: 6
  sub:
   uint: 3
   uint: 4""")
