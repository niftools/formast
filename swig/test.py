import formast

e = formast.Expr()
formast.parse_xml("test.txt", e)

class Visitor(formast.Visitor):
    def expr_uint(self, v):
        print(v)

    def expr_add(self, left, right):
        print "("
        self.expr(left)
        print "+"
        self.expr(right)
        print ")"

    def expr_sub(self, left, right):
        print "("
        self.expr(left)
        print "+"
        self.expr(right)
        print ")"

    def expr_mul(self, left, right):
        print "("
        self.expr(left)
        print "*"
        self.expr(right)
        print ")"

    def expr_div(self, left, right):
        print "("
        self.expr(left)
        print "/"
        self.expr(right)
        print ")"

    def expr_neg(self, right):
        print "-"
        self.expr(right)

    def expr_pos(self, right):
        print "+"
        self.expr(right)

visitor = Visitor()
visitor.expr(e)

