# create python package that includes all sources

if(WIN32)
  set(OSPATHSEP ";")
else()
  set(OSPATHSEP ":")
endif()

# copy source files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.cfg")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tox.ini.in" "${CMAKE_CURRENT_BINARY_DIR}/tox.ini")
file(COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
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
  DEPENDS ${SWIG_MODULE_formast_REAL_NAME}
)

add_test(python-tox
    ${PYTHON_EXECUTABLE} -c "import tox; tox.cmdline()")
