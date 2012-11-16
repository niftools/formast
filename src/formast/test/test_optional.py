import nose.tools
import formast

def test_optional():
    stats = formast.OptionalStats()
    nose.tools.assert_false(stats)
    nose.tools.assert_raises(ValueError, lambda: stats.get())

def test_optional_string_get_set():
    s = formast.OptionalString()
    s.set("hello")
    nose.tools.assert_equal(s.get(), "hello")
