import formast

parser = formast.XmlParser()
f = open("test.txt", "rb")
e = parser.parse_string(f.read())

class Printer(formast.Visitor):
    def expr_uint(self, v):
        print v,

    def expr_add(self, left, right):
        print "(",
        self.expr(left)
        print "+",
        self.expr(right)
        print ")",

    def expr_sub(self, left, right):
        print "(",
        self.expr(left)
        print "-",
        self.expr(right)
        print ")",

    def expr_mul(self, left, right):
        print "(",
        self.expr(left)
        print "*",
        self.expr(right)
        print ")",

    def expr_div(self, left, right):
        print "(",
        self.expr(left)
        print "/",
        self.expr(right)
        print ")",

    def expr_neg(self, right):
        print "- (",
        self.expr(right)
        print ")",

    def expr_pos(self, right):
        print "+",
        self.expr(right)

class Evaluator(formast.Visitor):
    def __init__(self):
        formast.Visitor.__init__(self)
        self.stack = []

    def expr(self, e):
        formast.Visitor.expr(self, e)
        print self.stack

    def expr_uint(self, v):
        self.stack.append(v)

    def expr_add(self, left, right):
        self.expr(left)
        self.expr(right)
        self.stack.append(self.stack.pop() + self.stack.pop())

    def expr_sub(self, left, right):
        self.expr(left)
        self.expr(right)
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

printer = Printer()
printer.expr(e)
print

evaluator = Evaluator()
evaluator.expr(e)
