Abstract Syntax Tree
====================

The abstract syntax tree is created from xml code,
via :cpp:func:`formast::Parser::parse_stream`
or :cpp:func:`formast::Parser::parse_string`.
The tree itself consists of a hierarchical structure of nodes.
This hierarchical structure is exposed via :cpp:class:`formast::Visitor`\ s.

.. cpp:class:: formast::Module

   Top level node of the abstract syntax tree.
   Contains :cpp:class:`formast::Class` nodes;
   use :cpp:func:`formast::Visitor::module` to iterate over them.

.. cpp:class:: formast::Stats

   A node containing field statements and conditional statements.
   Contains :cpp:class:`formast::Field` and :cpp:class:`formast::If` nodes;
   use :cpp:func:`formast::Visitor::stats` to iterate over them.

.. cpp:class:: formast::Expr

   Expression node.
   Use :cpp:func:`formast::Visitor::expr`
   to recursively inspect the expression.

.. cpp:type:: formast::Doc

   Documentation node. Currently, an alias for :cpp:class:`std::string`

.. cpp:class:: formast::Field

   Field (of a class) declaration.
   
   .. cpp:member:: std::string type_

      Type (a class name).

   .. cpp:member:: std::string name

      Name.

   .. cpp:member:: boost::optional<Doc> doc

      Documentation (optional).

   .. cpp:member:: boost::optional<Expr> arr1

      First array size (optional).

   .. cpp:member:: boost::optional<Expr> arr2

      Second array size (optional).

.. cpp:class:: formast::Class

   Class declaration.

   .. cpp:member:: std::string name

      Name.

   .. cpp:member:: boost::optional<std::string> base_name

      Name of base class.

   .. cpp:member:: boost::optional<Doc> doc

      Documentation (optional).

   .. cpp:member:: boost::optional<Stats> stats

      Field declarations.

.. cpp:class:: formast::If

   Declares fields to be conditional on some expression.

   .. cpp:member:: Expr expr

      The expression to evaluate.

   .. cpp:member:: Stats then

      Applicable fields when expression evaluates to true.

   .. cpp:member:: boost::optional<Stats> else_

      Applicable fields when expression evaluates to false (optional).

.. cpp:class:: boost::optional<T>

   See `boost documentation <http://www.boost.org/libs/optional>`_.

   The SWIG wrappers expose minimal, but sufficient, functionality:

   .. cpp:function:: T & get()

      Returns a reference to the contained value,
      throwing a SWIG ValueError exception if no value is contained.

   .. cpp:function:: bool is_initialized() const

      Returns ``true`` if the optional is initialized,
      ``false`` otherwise.

      .. note::

         The Python wrapper does not expose :cpp:func:`is_initialized`.
         Instead, it exposes :py:meth:`__nonzero__` (Python 2.x)
         and :py:meth:`__bool__` (Python 3.x) for testing whether
         a value is contained.
