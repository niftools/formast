cmake_minimum_required(VERSION 2.6)
project(FORMAST)
file(STRINGS "VERSION" VERSION)

# find boost
find_package(Boost 1.48.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${FORMAST_SOURCE_DIR}/include)

# build shared library
add_library(formastlib SHARED
    src/lib/visitor.cpp
    src/lib/parse.cpp
)
set_target_properties(formastlib
  PROPERTIES OUTPUT_NAME formast)

# build static library
add_library(formaststaticlib STATIC
    src/lib/visitor.cpp
    src/lib/parse.cpp
)
set_target_properties(formaststaticlib
  PROPERTIES
  OUTPUT_NAME formaststatic
  COMPILE_FLAGS -DFORMAST_STATIC
)

# build command line tool
add_executable(formastapp
    src/app/main.cpp
)
set_target_properties(formastapp
  PROPERTIES OUTPUT_NAME formast)
target_link_libraries(formastapp formastlib)

# build tests
enable_testing()
add_subdirectory(test)

# build wrappers
add_subdirectory(swig)
