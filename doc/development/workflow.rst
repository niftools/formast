Workflow
========

Set Up Build Folder
-------------------

Start buildenv, and type::

  cd ../build
  mkdir formast
  cd formast

Then, on Linux, simply type::

  cmake ../../workspace/formast

On Windows, you also have to specify the nmake generator::

  cmake -G "NMake Makefiles" ../../workspace/formast

You can control which wrappers to build from the command line,
using the
``BUILD_PYTHON``,
``BUILD_JAVA``,
``BUILD_CSHARP``, and
``BUILD_PHP`` options.
By default, Python and Java are ``ON``,
and the others are ``OFF``.
For example, if you would like to disable the Java wrapper build,
and enable the PHP wrapper build, you would write::

  cmake -G "NMake Makefiles" ../../workspace/formast -DBUILD_JAVA=OFF -DBUILD_PHP=ON

Compile
-------

In buildenv, ``build/formast`` folder, run on Linux::

  make
  make package

or, on Windows::

  nmake
  nmake package

Wrapper packages for Java and Python will also be generated, you will
find those in the build folders ``swig/java`` and ``swig/python/package``.

By default, SWIG only creates a source package for Python.
To create a binary package for Python, run ``python setup.py bdist_egg``
from the ``src/formast/swig/python/package`` build folder.

Run Regression Tests
--------------------

In buildenv, ``build/formast`` folder, run::

  ctest -V

Generate Documentation
----------------------

Run the following in a buildenv (Windows) or terminal (Fedora),
``workspace/formast`` folder::

  cd doc
  make html

To view the docs, open ``docs/_build/html/index.html``.
