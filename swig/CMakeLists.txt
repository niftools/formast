find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "-Wall")

add_subdirectory(python)

add_subdirectory(java)

# CSharp target needs a bug fixed for SWIG; see patch reported upstream:
# https://sourceforge.net/tracker/?func=detail&aid=3571029&group_id=1645&atid=301645
#add_subdirectory(csharp)

#add_subdirectory(php)
