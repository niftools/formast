option(BUILD_PYTHON "Build Python wrapper." ON)
option(BUILD_JAVA "Build Java wrapper." ON)
option(BUILD_CSHARP "Build C# wrapper." OFF) # needs bugfix, see below
option(BUILD_PHP "Build PHP wrapper." OFF) # secondary target

if(BUILD_PYTHON OR BUILD_JAVA OR BUILD_CSHARP OR BUILD_PHP)
  find_package(SWIG REQUIRED)
  include(${SWIG_USE_FILE})
  set(CMAKE_SWIG_FLAGS "-Wall")
  if(BUILD_PYTHON)
    add_subdirectory(python)
  endif()
  if(BUILD_JAVA)
    add_subdirectory(java)
  endif()
  # CSharp target needs a bug fixed for SWIG; see patch reported upstream:
  # https://sourceforge.net/tracker/?func=detail&aid=3571029&group_id=1645&atid=301645
  if(BUILD_CSHARP)
    add_subdirectory(csharp)
  endif()
  if(BUILD_PHP)
    add_subdirectory(php)
  endif()
endif()
