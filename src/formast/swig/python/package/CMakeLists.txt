# create python package that includes all sources

# copy source files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tox.ini.in" "${CMAKE_CURRENT_BINARY_DIR}/tox.ini")
file(COPY 
    ${formast_SOURCE_DIR}/README.rst
    ${formast_SOURCE_DIR}/src/formast/lib
    ${formast_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
file(COPY
    @Boost_INCLUDE_DIR@/boost/algorithm
    @Boost_INCLUDE_DIR@/boost/fusion
    @Boost_INCLUDE_DIR@/boost/lexical_cast.hpp
    @Boost_INCLUDE_DIR@/boost/optional.hpp
    @Boost_INCLUDE_DIR@/boost/optional
    @Boost_INCLUDE_DIR@/boost/shared_ptr.hpp
    @Boost_INCLUDE_DIR@/boost/spirit
    @Boost_INCLUDE_DIR@/boost/variant
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/boost
)

# target for source package
set(FORMAST_PYTHON_SDIST_FILE dist/formast-${VERSION}.zip)
add_custom_target(formast-python-sdist ALL DEPENDS ${FORMAST_PYTHON_SDIST_FILE})

# generate source package
# must copy swig generated files into current directory
add_custom_command(
  OUTPUT ${FORMAST_PYTHON_SDIST_FILE}
  COMMENT "Creating python sdist file..."
  COMMAND ${CMAKE_COMMAND} -E make_directory formast/
  COMMAND ${CMAKE_COMMAND} -E copy ../formast.py formast/__init__.py
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.cxx .
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.h .
  COMMAND ${PYTHON_EXECUTABLE} setup.py sdist --format=zip --quiet
  COMMAND ${PYTHON_EXECUTABLE} setup.py bdist --format=zip --quiet
  DEPENDS ${SWIG_MODULE_formast_REAL_NAME}
)

add_test(python-tox
    ${PYTHON_EXECUTABLE} -c "import tox; tox.cmdline()")
