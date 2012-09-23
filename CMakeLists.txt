cmake_minimum_required(VERSION 2.6)
project(FORMAST)
file(STRINGS "VERSION" VERSION)

# find boost
find_package(Boost 1.48.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${FORMAST_SOURCE_DIR}/include)

# build library
add_library(formast-lib SHARED
    src/lib/visitor.cpp
    src/lib/parse.cpp
)
set_target_properties(formast-lib
  PROPERTIES OUTPUT_NAME formast)

# build command line tool
add_executable(formast-app
    src/app/main.cpp
)
set_target_properties(formast-app
  PROPERTIES OUTPUT_NAME formast)
target_link_libraries(formast-app formast-lib)

# build tests
enable_testing()
add_subdirectory(test)

# build wrappers
add_subdirectory(swig)
