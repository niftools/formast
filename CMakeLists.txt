cmake_minimum_required(VERSION 2.6)
project(FORMAST)
file(STRINGS "VERSION" VERSION)

# configuration
configure_file(
    ${FORMAST_SOURCE_DIR}/Doxyfile.in
    ${FORMAST_SOURCE_DIR}/Doxyfile)

# find boost
find_package(Boost 1.48.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${FORMAST_SOURCE_DIR}/include)

# build library
add_library(formast-lib SHARED
    src/formast.cpp
    src/formast/visitor.cpp
    src/formast/parse.cpp
    src/formast/printer.cpp
)
set_target_properties(formast-lib
  PROPERTIES OUTPUT_NAME formast)

# build command line tool
add_executable(formast-cmd
    src/main.cpp
)
set_target_properties(formast-cmd
  PROPERTIES OUTPUT_NAME formast)
target_link_libraries(formast-cmd formast-lib)

# build tests
enable_testing()
add_subdirectory(test)

# build wrappers
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "-Wall")
set_source_files_properties(swig/formast.i PROPERTIES CPLUSPLUS ON)
add_subdirectory(python)
add_subdirectory(java)
#add_subdirectory(csharp)
#add_subdirectory(php)
