Parser
======

.. cpp:class:: formast::Parser

   Abstract base class for parsers.

   .. cpp:function:: void parse_stream(std::istream & is, Module & module)

      Parse input stream into a :cpp:class:`Module`.

   .. cpp:function:: void parse_string(std::string const & s, Module & module)

      Parse string into a :cpp:class:`Module`.

.. cpp:class:: formast::XmlParser

   Bases: :cpp:class:`formast::Parser`

   Parser for the niftools xml format.
