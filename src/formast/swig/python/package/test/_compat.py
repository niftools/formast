"""Hacks for older python versions."""

import sys
import nose.tools

def assert_isinstance(x, y):
    assert isinstance(x, y)

def assert_in(x, y):
    assert x in y

if sys.version_info[:2] <= (2, 6) or sys.version_info[:2] == (3, 0):
    nose.tools.assert_list_equal = nose.tools.assert_equal
    nose.tools.assert_multi_line_equal = nose.tools.assert_equal
    nose.tools.assert_isinstance = assert_isinstance
    nose.tools.assert_in = assert_in
