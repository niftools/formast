option(BUILD_SHARED_LIBRARY "Build shared library." ON)
option(BUILD_STATIC_LIBRARY "Build static library." ON)

set(FORMAST_LIBRARY_SOURCES
  parse.cpp
  stats.cpp
  top.cpp
  visitor.cpp
)

if(BUILD_SHARED_LIBRARY)
  add_library(formastlib SHARED
    ${FORMAST_LIBRARY_SOURCES}
  )

  set_target_properties(formastlib
    PROPERTIES OUTPUT_NAME formast
  )

  install(
    TARGETS formastlib
    RUNTIME DESTINATION bin COMPONENT libraries
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
  )
endif()

if(BUILD_STATIC_LIBRARY)
  add_library(formaststaticlib STATIC
    ${FORMAST_LIBRARY_SOURCES}
  )

  set_target_properties(formaststaticlib
    PROPERTIES
    OUTPUT_NAME formast_static
    COMPILE_FLAGS -DFORMAST_STATIC
  )

  install(
    TARGETS formaststaticlib
    RUNTIME DESTINATION bin COMPONENT libraries
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
  )
endif()
