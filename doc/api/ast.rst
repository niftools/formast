Abstract Syntax Tree
====================

For the time being, FormAST exposes just a single node type,
namely :cpp:class:`formast::Expr`,
for expressions.
Expressions are created from xml code,
via :cpp:func:`formast::Parser::parse_stream`
or :cpp:func:`formast::Parser::parse_string`.

Nodes of the abstract syntax tree have no public methods.
Instead, the structure of the nodes
is exposed via :cpp:class:`formast::Visitor`\ s.

.. cpp:class:: formast::Expr

   Expression node of the abstract syntax tree.

