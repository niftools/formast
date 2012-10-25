# build python package including all sources
# and test it

# copy source files
set(FORMAST_USE_SYSTEM_LIBS False)
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
    @Boost_INCLUDE_DIR@/boost
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include
    # try to include only what we need
    FILES_MATCHING
    PATTERN "boost/*.hpp"
    REGEX "boost/algorithm/.*"
    REGEX "boost/archive/.*"
    REGEX "boost/bind/.*"
    REGEX "boost/config/.*"
    REGEX "boost/concept/.*"
    REGEX "boost/detail/.*"
    REGEX "boost/exception/.*"
    REGEX "boost/function/.*"
    REGEX "boost/function_types/.*"
    REGEX "boost/functional/.*"
    REGEX "boost/fusion/.*"
    REGEX "boost/iostreams/.*"
    REGEX "boost/iterator/.*"
    REGEX "boost/math/.*"
    REGEX "boost/mpl/.*"
    REGEX "boost/multi_index/.*"
    REGEX "boost/optional/.*"
    REGEX "boost/preprocessor/.*"
    REGEX "boost/property_tree/.*"
    REGEX "boost/proto/.*"
    REGEX "boost/range/.*"
    REGEX "boost/regex/.*"
    REGEX "boost/smart_ptr/.*"
    REGEX "boost/serialization/.*"
    REGEX "boost/spirit/.*"
    REGEX "boost/typeof/.*"
    REGEX "boost/tuple/.*"
    REGEX "boost/type_traits/.*"
    REGEX "boost/utility/.*"
    REGEX "boost/variant/.*"
    REGEX "boost/spirit/home/classic/.*" EXCLUDE
    REGEX "boost/spirit/home/lex/.*" EXCLUDE
    REGEX "boost/spirit/home/support/detail/lexer/.*" EXCLUDE
    REGEX "boost/spirit/include/classic.*" EXCLUDE
    REGEX "boost/spirit/include/phoenix1.*" EXCLUDE
)

# target for source package
set(FORMAST_PYTHON_SDIST_FILE dist/formast-${VERSION}.zip)
add_custom_target(formast-python-sdist ALL DEPENDS ${FORMAST_PYTHON_SDIST_FILE})

# generate source package
# must copy swig generated files into current directory
add_custom_command(
  OUTPUT ${FORMAST_PYTHON_SDIST_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory formast/
  COMMAND ${CMAKE_COMMAND} -E copy ../formast.py formast/__init__.py
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.cxx .
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.h .
  COMMENT "Creating python sdist file..."
  COMMAND ${PYTHON_EXECUTABLE} setup.py sdist --format=bztar --quiet
  COMMENT "Creating python bdist_egg file..."
  COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_egg --quiet
  DEPENDS ${SWIG_MODULE_formast_REAL_NAME}
)

add_test(python-tox
    ${PYTHON_EXECUTABLE} -c "import tox; tox.cmdline()")
