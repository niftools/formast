option(BUILD_SHARED_LIBRARY "Build shared library." ON)
option(BUILD_STATIC_LIBRARY "Build static library." ON)

set(FORMAST_LIBRARY_SOURCES
  @FORMAST_SOURCE_DIR@/src/lib/visitor.cpp
  @FORMAST_SOURCE_DIR@/src/lib/parse.cpp
)

if(BUILD_SHARED_LIBRARY)
  add_subdirectory(shared)
endif()

if(BUILD_STATIC_LIBRARY)
  add_subdirectory(static)
endif()
