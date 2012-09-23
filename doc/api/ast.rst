Abstract Syntax Tree
====================

For the time being, FormAST exposes just a single node type,
namely :cpp:class:`formast::Expr`,
for expressions.
Expressions are created from xml code,
via :cpp:member:`formast::Parser::parse_string`.

.. cpp:class:: formast::Expr
