Parser
======

.. cpp:class:: formast::Parser

   Abstract base class for parsers.

   .. cpp:function:: Expr parse_stream(std::istream & is)

      Parse input stream into an abstract syntax tree.

   .. cpp:function:: Expr parse_string(std::string const & s)

      Parse string into an abstract syntax tree.

.. cpp:class:: formast::XmlParser

   Bases: :cpp:class:`formast::Parser`

   Parser for the niftools xml format.
