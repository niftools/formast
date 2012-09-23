Workflow
========

Set Up Build Folder
-------------------

Start buildenv, and type::

  cd ../build
  mkdir formast
  cd formast
  cmake ../../workspace/formast

.. warning::

   On Windows, if you have multiple compilers installed, CMake may not
   select the correct one by default.
   Also Windows needs ``-G ...`` to ensure it generates nmake files.

.. todo::

   Figure out a way to do everything properly from the command line
   on Windows.

Compile
-------

In buildenv, ``build/formast`` folder, run::

  make

or, on Windows::

  nmake

Wrapper packages for Java and Python will also be generated, you will
find those in ``swig/java`` and ``swig/python/package``.

By default, SWIG only creates a source package for Python.
To create a binary package for Python, run ``python setup.py bdist``.

Run Regression Tests
--------------------

In buildenv, ``build/formast`` folder, run::

  ctest -V

Generate Documentation
----------------------

Run the following in a buildenv (Windows) or terminal (Fedora),
``workspace/formast`` folder::

  cd docs
  make html

To view the docs, open ``docs/_build/html/index.html``.
