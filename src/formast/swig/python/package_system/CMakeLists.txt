# build python package using system libs

# OSPATH and OSPATHSEP for setup.cfg.in and tox.ini.in
set(OSPATH "$ENV{PATH}")
if(WIN32)
  set(OSPATHSEP ";")
else()
  set(OSPATHSEP ":")
endif()

# copy source files
set(FORMAST_USE_SYSTEM_LIBS True)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../package/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.cfg")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tox.ini.in" "${CMAKE_CURRENT_BINARY_DIR}/tox.ini")

# target for source package
set(FORMAST_PYTHON_SDIST_SYSTEM_LIBS_FILE dist/formast-${VERSION}.zip)
add_custom_target(formast-python-system-libs-sdist ALL DEPENDS ${FORMAST_PYTHON_SDIST_SYSTEM_LIBS_FILE})
file(COPY
    ${formast_SOURCE_DIR}/README.rst
    ${CMAKE_CURRENT_SOURCE_DIR}/../package/MANIFEST.in
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# generate source package
# must copy swig generated files into current directory
add_custom_command(
  OUTPUT ${FORMAST_PYTHON_SDIST_SYSTEM_LIBS_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory formast/
  COMMAND ${CMAKE_COMMAND} -E copy ../formast.py formast/__init__.py
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.cxx .
  COMMAND ${CMAKE_COMMAND} -E copy ../formastPYTHON_wrap.h .
  COMMENT "Creating python sdist file (with system libs)..."
  COMMAND ${PYTHON_EXECUTABLE} setup.py sdist --format=zip --quiet
  COMMENT "Creating python bdist file (with system libs)..."
  COMMAND ${PYTHON_EXECUTABLE} setup.py bdist --format=zip --quiet
  DEPENDS ${SWIG_MODULE_formast_REAL_NAME}
)

add_test(python-system-libs-tox
    ${PYTHON_EXECUTABLE} -c "import tox; tox.cmdline()")
