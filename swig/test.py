import formast

e = formast.Expr()
formast.parse_xml("test.txt", e)

class Visitor(formast.Visitor):
    def uint(self, v):
        print(v)

    def add(self, left, right):
        print "("
        self.expr(left)
        print "+"
        self.expr(right)
        print ")"

    def sub(self, left, right):
        print "("
        self.expr(left)
        print "+"
        self.expr(right)
        print ")"

    def mul(self, left, right):
        print "("
        self.expr(left)
        print "*"
        self.expr(right)
        print ")"

    def div(self, left, right):
        print "("
        self.expr(left)
        print "/"
        self.expr(right)
        print ")"

    def neg(self, right):
        print "-"
        self.expr(right)

    def pos(self, right):
        print "+"
        self.expr(right)

visitor = Visitor()
print visitor.expr(e)
