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

    def module_class(self, c):
        self.print_("class:")
        self.level += 1
        self.print_("name: %s" % c.name)
        if c.base_name:
            self.print_("base_name: %s" % c.base_name.get())
        if c.stats:
            self.stats(c.stats.get())
        self.level -= 1

    def module_enum(self, e):
        self.print_("enum:")
        self.level += 1
        self.print_("name: %s" % e.name)
        self.print_("base_name: %s" % e.base_name)
        if e.stats:
            self.enum_stats(e.stats.get())
        self.level -= 1

    def stats(self, s):
        self.print_("stats:")
        self.level += 1
        formast.Visitor.stats(self, s)
        self.level -= 1

    def stats_field(self, a):
        self.print_("field:")
        self.level += 1
        self.print_("type: %s" % a.type_)
        self.print_("name: %s" % a.name)
        self.level -= 1

    def stats_if(self, if_):
        self.print_("if:")
        self.level += 1
        self.expr(if_.expr)
        self.print_("then:")
        self.level += 1
        self.stats(if_.then)
        self.level -= 1
        if if_.else_:
            self.print_("else:")
            self.level += 1
            self.stats(if_.else_.get())
            self.level -= 1
        self.level -= 1

    def enum_stats(self, s):
        self.print_("stats:")
        self.level += 1
        formast.Visitor.enum_stats(self, s)
        self.level -= 1

    def enum_stats_const(self, c):
        self.print_("const:")
        self.level += 1
        self.print_("name: %s" % c.name)
        self.print_("value: %s" % c.value)
        self.level -= 1

    def expr_uint(self, v):
        self.print_("uint: %s" % v)

    def expr_id(self, i):
        self.print_("id: %s" % i)

    def expr_pos(self, right):
        self.print_("pos:")
        self.level += 1
        self.expr(right)
        self.level -= 1

    def expr_neg(self, right):
        self.print_("neg:")
        self.level += 1
        self.expr(right)
        self.level -= 1

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

    def expr_mod(self, left, right):
        self.print_("mod:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_pow(self, left, right):
        self.print_("pow:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_logical_and(self, left, right):
        self.print_("logical_and:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_logical_or(self, left, right):
        self.print_("logical_or:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_logical_not(self, right):
        self.print_("not:")
        self.level += 1
        self.expr(right)
        self.level -= 1

    def expr_bitwise_and(self, left, right):
        self.print_("bitwise_and:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_bitwise_or(self, left, right):
        self.print_("bitwise_or:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_bitwise_xor(self, left, right):
        self.print_("bitwise_xor:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_eq(self, left, right):
        self.print_("compare_eq:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_ne(self, left, right):
        self.print_("compare_ne:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_gt(self, left, right):
        self.print_("compare_gt:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_lt(self, left, right):
        self.print_("compare_lt:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_ge(self, left, right):
        self.print_("compare_ge:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_compare_le(self, left, right):
        self.print_("compare_le:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_shift_left(self, left, right):
        self.print_("shift_left:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

    def expr_shift_right(self, left, right):
        self.print_("shift_right:")
        self.level += 1
        self.expr(left)
        self.expr(right)
        self.level -= 1

class TestPrint:
    def setup(self):
        self.parser = formast.XmlParser()
        self.printer = Printer()

    def check(self, inp , out):
        module = formast.Module()
        self.parser.parse_string(self.make_input_from_cond(inp), module)
        self.printer.module(module)
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
   then:
    stats:
     field:
      type: uint
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
   then:
    stats:
     field:
      type: uint
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
   then:
    stats:
     field:
      type: uint
      name: test""")

    def test_conditioning(self):
        module = formast.Module()
        self.parser.parse_string("""
<niftoolsxml>
  <compound name="Test">
    <add name="Has Image" type="bool" />
    <add name="PS2 L" type="short" cond="Has Image" ver1="3.03" ver2="10.2.0.0" />
  </compound>
</niftoolsxml>
""", module)
        self.printer.module(module)
        # note: 50332416 = 0x03000300 and 167903232 = 0x0A020000
        nose.tools.assert_equal(str(self.printer), """\
class:
 name: Test
 stats:
  field:
   type: bool
   name: Has Image
  if:
   logical_and:
    logical_and:
     id: Has Image
     compare_ge:
      id: Version
      uint: 50332416
    compare_le:
     id: Version
     uint: 167903232
   then:
    stats:
     field:
      type: short
      name: PS2 L""")

    def test_enum(self):
        module = formast.Module()
        self.parser.parse_string("""
<niftoolsxml>
  <enum name="AlphaFormat" storage="uint">
    An unsigned 32-bit integer, describing how transparency is handled in a texture.
    <option value="0" name="ALPHA_NONE">No alpha blending; the texture is fully opaque.</option>
    <option value="1" name="ALPHA_BINARY">Texture is either fully transparent or fully opaque.  There are no partially transparent areas.</option>
    <option value="2" name="ALPHA_SMOOTH">Full range of alpha values can be used from fully transparent to fully opaque including all partially transparent values in between.</option>
    <option value="3" name="ALPHA_DEFAULT">Use default setting.</option>
  </enum>
</niftoolsxml>
""", module)
        self.printer.module(module)
        nose.tools.assert_equal(str(self.printer), """\
enum:
 name: AlphaFormat
 base_name: uint
 stats:
  const:
   name: ALPHA_NONE
   value: 0
  const:
   name: ALPHA_BINARY
   value: 1
  const:
   name: ALPHA_SMOOTH
   value: 2
  const:
   name: ALPHA_DEFAULT
   value: 3""")
