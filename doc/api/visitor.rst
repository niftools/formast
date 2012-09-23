Visitor
=======

Rather than exposing the data structure
representing the abstract syntax tree directly,
FormAST exposes the structure by means of
`visitation <http://en.wikipedia.org/wiki/Visitor_pattern>`_.
In practice, visitation provides every means for evaluating
the abstract syntax tree,
or for compiling it into another tree structure
for whatever sofisticated purpose.
Moreover, the visitor model allows new language features to be added
with minimal change to the public API.

Simply derive a subclass from :cpp:class:`formast::Visitor`
and override the desired methods.

.. cpp:class:: formast::Visitor

   .. cpp:function:: void expr(formast::Expr const & e)
   .. cpp:function:: void expr_uint(boost::uint64_t const & n)
   .. cpp:function:: void expr_id(std::string const & i)
   .. cpp:function:: void expr_pos(formast::Expr const & right)
   .. cpp:function:: void expr_neg(formast::Expr const & right)
   .. cpp:function:: void expr_add(formast::Expr const & left, formast::Expr const & right)
   .. cpp:function:: void expr_sub(formast::Expr const & left, formast::Expr const & right)
   .. cpp:function:: void expr_mul(formast::Expr const & left, formast::Expr const & right)
   .. cpp:function:: void expr_div(formast::Expr const & left, formast::Expr const & right)
