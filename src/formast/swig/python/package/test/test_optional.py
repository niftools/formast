import nose.tools
import formast

def test_optional():
    stats = formast.OptionalStats()
    nose.tools.assert_false(stats)
    nose.tools.assert_raises(ValueError, lambda: stats.get())
