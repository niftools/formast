import codecs
import codegen
import codegen.integers
import formast
import nose.tools
import os.path
import unittest

unittest.TestCase.maxDiff = None

def test_codegen():
    top = formast.Top()
    with codecs.open("codegen/integers.xml", "rb", "utf8") as stream:
        formast.XmlParser().parse_string(stream.read(), top)
    codegen_module = codegen.CodeGenModule()
    codegen_module.top(top)
    with codecs.open("codegen/integers.py", "rb", "utf8") as stream:
        nose.tools.assert_multi_line_equal(stream.read(), str(codegen_module))

def test_ver0_1():
    data = codegen.integers.IntegerData()
    with open("test_ver0_1.integers", "rb") as stream:
        data.read(stream)
        eof_check = stream.read(1)
    nose.tools.assert_equal(eof_check, '')
    nose.tools.assert_equal(data.version, 0)
    nose.tools.assert_equal(data.has_integers, 1)
    nose.tools.assert_equal(data.has_integers_2, 0)
    nose.tools.assert_equal(data.num_integers, 3)
    nose.tools.assert_list_equal(data.integers, [3, 1, 4])
    nose.tools.assert_equal(data.num_integers_2, 0)
    nose.tools.assert_list_equal(data.integers_2, [])

def test_ver2_1():
    data = codegen.integers.IntegerData()
    with open("test_ver2_1.integers", "rb") as stream:
        data.read(stream)
        eof_check = stream.read(1)
    nose.tools.assert_equal(eof_check, '')
    nose.tools.assert_equal(data.version, 2)
    nose.tools.assert_equal(data.has_integers, 1)
    nose.tools.assert_equal(data.has_integers_2, 1)
    nose.tools.assert_equal(data.num_integers, 3)
    nose.tools.assert_list_equal(data.integers, [3, 1, 4])
    nose.tools.assert_equal(data.num_integers_2, 5)
    nose.tools.assert_list_equal(data.integers_2, [2, 7, 1, 8, 2])
