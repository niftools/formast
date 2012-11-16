cmake_minimum_required(VERSION 2.6)
project(formast)
file(STRINGS "VERSION" VERSION)

# activate c++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
  DESTINATION include
)

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
include(CPack)
