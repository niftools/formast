# build wrapper

set(Python_ADDITIONAL_VERSIONS 3.2mu) # cmake bug: it is unaware of ABI tags
find_package(PythonLibs 3.2 REQUIRED)
find_package(PythonInterp 3.2 REQUIRED)
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_PATH})
set_source_files_properties(formast.i PROPERTIES CPLUSPLUS ON)
swig_add_module(formast python formast.i)
swig_link_libraries(formast formast-lib ${PYTHON_LIBRARIES})

# create python package that includes all sources
# swig generated files are already in ${CMAKE_CURRENT_BINARY_DIR}
# still need to copy include files and source files

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

file(COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in
    ${CMAKE_CURRENT_SOURCE_DIR}/tox.ini
    ${FORMAST_SOURCE_DIR}/src
    ${FORMAST_SOURCE_DIR}/include
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    PATTERN "src/app" EXCLUDE
    )

# target for source package
set(FORMAST_PYTHON_SDIST_FILE dist/formast-${VERSION}.zip)
add_custom_target(formast-python-sdist ALL DEPENDS ${FORMAST_PYTHON_SDIST_FILE})

# generate source package
add_custom_command(
  OUTPUT ${FORMAST_PYTHON_SDIST_FILE}
  COMMENT "Creating python sdist file..."
  COMMAND ${PYTHON_EXECUTABLE} setup.py sdist --format=zip --quiet
  DEPENDS ${SWIG_MODULE_formast_REAL_NAME}
)

add_test(python-tox
    ${PYTHON_EXECUTABLE} -c "import tox; tox.cmdline()")
