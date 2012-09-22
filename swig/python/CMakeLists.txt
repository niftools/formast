# build wrapper

set(Python_ADDITIONAL_VERSIONS 3.2mu) # cmake bug: it is unaware of ABI tags
find_package(PythonLibs 3.2 REQUIRED)
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_PATH})
set_source_files_properties(formast.i PROPERTIES CPLUSPLUS ON)
swig_add_module(formast python formast.i)
swig_link_libraries(formast formast-lib ${PYTHON_LIBRARIES})

# create python package that includes all sources
# swig generated files are already in ${CMAKE_CURRENT_BINARY_DIR}
# still need to copy include files and other source files

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

file(COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in
    ${FORMAST_SOURCE_DIR}/src
    ${FORMAST_SOURCE_DIR}/include
    DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}
    )
