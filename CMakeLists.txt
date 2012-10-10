cmake_minimum_required(VERSION 2.6)
project(formast)
file(STRINGS "VERSION" VERSION)

# find boost
find_package(Boost 1.48.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${formast_SOURCE_DIR}/include)

# build libraries
add_subdirectory(lib)

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

# install headers, sources, and application
# (library install is done in lib/shared/CMakeLists.txt)
install(
    DIRECTORY include/
    DESTINATION include
    COMPONENT headers
)
install(
    DIRECTORY @Boost_INCLUDE_DIR@/boost/spirit
    DESTINATION include/boost
    COMPONENT headers-boost
)
install(
    FILES
    @Boost_INCLUDE_DIR@/boost/shared_ptr.hpp
    DESTINATION include/boost
    COMPONENT headers-boost
)
install(
    DIRECTORY src swig
    DESTINATION src/formast
    COMPONENT sources
)
install(
    TARGETS formastapp
    RUNTIME DESTINATION bin
    COMPONENT applications
)

# enable CPack
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR ZIP)
include(CPack)
