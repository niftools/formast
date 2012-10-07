import nose.tools
nose.tools.assert_multi_line_equal.im_class.maxDiff = None

import codegen
import formast

def test_codegen():
    top = formast.Top()
    with open("integers.xml", "rb") as stream:
        formast.XmlParser().parse_string(stream.read(), top)
    codegen_module = codegen.CodeGenModule()
    codegen_module.top(top)
    with open("integers.py", "rb") as stream:
        nose.tools.assert_multi_line_equal(stream.read(), str(codegen_module))
