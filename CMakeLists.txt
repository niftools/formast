cmake_minimum_required(VERSION 2.6)
project(formast)
file(STRINGS "VERSION" VERSION)

# install dirs
if(INCLUDE_INSTALL_DIR)
else()
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif()
if(LIB_INSTALL_DIR)
else()
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
endif()
if(BIN_INSTALL_DIR)
else()
set(BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
endif()

# find boost
find_package(Boost 1.48.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${formast_SOURCE_DIR}/include)

# build libraries
add_subdirectory(src/formast/lib)

# build command line tool
add_subdirectory(src/formast/bin)

# build tests
enable_testing()
add_subdirectory(src/formast/test)

# build wrappers
add_subdirectory(src/formast/swig)

# install headers, sources, and application
# (library install is done in src/formast/lib/CMakeLists.txt)
install(
  DIRECTORY include/
  DESTINATION ${INCLUDE_INSTALL_DIR}
)

# spec files for fedora
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/formast.spec.in" "${CMAKE_CURRENT_BINARY_DIR}/formast.spec")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mingw-formast.spec.in" "${CMAKE_CURRENT_BINARY_DIR}/mingw-formast.spec")

# enable CPack
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR ZIP)
if(MSVC)
  include(CMakeDetermineVSServicePack)
  DetermineVSServicePack(VCTAG)
  if(VCTAG)
    if(CMAKE_CL_64)
      set(CPACK_SYSTEM_NAME win64-${VCTAG})
    else()
      set(CPACK_SYSTEM_NAME win32-${VCTAG})
    endif()
  endif()
endif()
set(CPACK_SOURCE_GENERATOR ZIP)
include(CPack)
