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

    def top_class(self, c):
        self.print_("class:")
        self.level += 1
        self.print_("name: %s" % c.name)
        if c.base_name.is_initialized():
            self.print_("base_name: %s" % c.base_name.get())
        if c.stats.is_initialized():
            self.stats(c.stats.get())
        self.level -= 1

    def stats(self, s):
        self.print_("stats:")
        self.level += 1
        formast.Visitor.stats(self, s)
        self.level -= 1

    def stats_attr(self, a):
        self.print_("attr:")
        self.level += 1
        self.print_("class_name: %s" % a.class_name)
        self.print_("name: %s" % a.name)

    def stats_if_elifs_else(self, i):
        first = True
        for if_ in i.ifs_:
            if first:
                self.print_("if:")
                first = False
            else:
                self.print_("elif:")
            self.level += 1
            self.expr(if_.expr)
            self.stats(if_.stats)
            self.level -= 1

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
        top = formast.Top()
        self.parser.parse_string(self.make_input_from_cond(inp), top)
        self.printer.top(top)
        nose.tools.assert_equal(str(self.printer), out)

    def make_input_from_cond(self, cond):
        return """
<niftoolsxml>
  <compound name="Test">
    <add name="test" type="uint" cond="%s" />
  </compound>
</niftoolsxml>
""" % cond

    def test_uint(self):
        self.check("99",
                   """\
class:
 name: Test
 stats:
  if:
   uint: 99
   stats:
    attr:
     class_name: uint
     name: test""")
    
    def test_add(self):
        self.check("1+2", """\
class:
 name: Test
 stats:
  if:
   add:
    uint: 1
    uint: 2
   stats:
    attr:
     class_name: uint
     name: test""")

    def test_complicated(self):
        self.check("1+(2*3+4)*6/(3-4)", """\
class:
 name: Test
 stats:
  if:
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
      uint: 4
   stats:
    attr:
     class_name: uint
     name: test""")
