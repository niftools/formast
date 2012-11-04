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

Simply derive a subclass from :cpp:class:`formast::Visitor`
and overload the desired methods.

.. cpp:class:: formast::Visitor

   Visitor for the abstract syntax tree.

   .. cpp:function:: void module(Module const & module)

      Visits a :cpp:class:`formast::Module`,
      dispatching each of its children
      to the appropriate ``module_...`` method.

   .. cpp:function:: void module_class(Class const & class_)
   .. cpp:function:: void module_enum(Enum const & enum_)

   .. cpp:function:: void stats(Stats const & stats)

      Visits a :cpp:class:`formast::Stats`,
      dispatching each of its children
      to the appropriate ``stats_...`` method.

   .. cpp:function:: void stats_field(Field const & field)
   .. cpp:function:: void stats_if(If const & if_)

   .. cpp:function:: void enum_stats(EnumStats const & stats)

      Visits a :cpp:class:`formast::EnumStats`,
      dispatching each of its children
      to the appropriate ``enum_stats_...`` method.

   .. cpp:function:: void enum_stats_const(EnumConst const & const_)

   .. cpp:function:: void expr(Expr const & e)

      Visits an :cpp:class:`formast::Expr`,
      dispatching
      to the appropriate ``expr_...`` method.

   .. cpp:function:: void expr_uint(boost::uint64_t const & n)
   .. cpp:function:: void expr_id(std::string const & i)
   .. cpp:function:: void expr_pos(Expr const & right)
   .. cpp:function:: void expr_neg(Expr const & right)
   .. cpp:function:: void expr_add(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_sub(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_mul(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_div(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_mod(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_pow(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_logical_and(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_logical_or(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_logical_not(Expr const & right)
   .. cpp:function:: void expr_bitwise_and(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_bitwise_or(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_bitwise_xor(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_eq(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_ne(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_gt(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_lt(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_ge(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_compare_le(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_shift_left(Expr const & left, Expr const & right)
   .. cpp:function:: void expr_shift_right(Expr const & left, Expr const & right)
