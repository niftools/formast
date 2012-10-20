# build wrapper

set(Python_ADDITIONAL_VERSIONS 3.2mu) # cmake bug: it is unaware of ABI tags
find_package(PythonLibs 3.2 REQUIRED)
find_package(PythonInterp 3.2 REQUIRED)
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_PATH})
set_source_files_properties(formast.i PROPERTIES CPLUSPLUS ON)
swig_add_module(formast python formast.i)
swig_link_libraries(formast formastlib ${PYTHON_LIBRARIES})

# build package

add_subdirectory(package)
add_subdirectory(package_system)
