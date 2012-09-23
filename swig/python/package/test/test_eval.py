"""Test parser with an evaluator-like visitor."""

import nose.tools
import formast

class Evaluator(formast.Visitor):
    def __init__(self):
        formast.Visitor.__init__(self)
        self.stack = []

    def expr_uint(self, v):
        self.stack.append(v)

    def expr_add(self, left, right):
        self.expr(left)
        self.expr(right)
        self.stack.append(self.stack.pop() + self.stack.pop())

    def expr_sub(self, left, right):
        self.expr(right)
        self.expr(left) # will pop first!
        self.stack.append(self.stack.pop() - self.stack.pop())

    def expr_mul(self, left, right):
        self.expr(left)
        self.expr(right)
        self.stack.append(self.stack.pop() * self.stack.pop())

    def expr_div(self, left, right):
        self.expr(right)
        self.expr(left) # will pop first!
        self.stack.append(self.stack.pop() // self.stack.pop())

    def expr_neg(self, right):
        self.expr(right)
        self.stack.append(-self.stack.pop())

    def expr_pos(self, right):
        pass

class TestEvaluator:
    def setup(self):
        self.parser = formast.XmlParser()
        self.evaluator = Evaluator()

    def check(self, inp , out):
        expr = self.parser.parse_string(inp)
        self.evaluator.expr(expr)
        nose.tools.assert_equal(len(self.evaluator.stack), 1)
        nose.tools.assert_equal(self.evaluator.stack[0], out)

    def test_uint(self):
        self.check("99", 99)
    
    def test_add(self):
        self.check("1+2", 3)

    def test_complicated(self):
        self.check("1+(2*3+4)*6/(3-4)", -59)
