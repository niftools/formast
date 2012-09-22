find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "-Wall")

add_subdirectory(python)
add_subdirectory(java)
#add_subdirectory(csharp)
#add_subdirectory(php)
