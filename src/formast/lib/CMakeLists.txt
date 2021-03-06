option(BUILD_SHARED_LIBRARY "Build shared library." ON)
option(BUILD_STATIC_LIBRARY "Build static library." ON)

set(FORMAST_LIBRARY_SOURCES
  enum_stats.cpp
  expr.cpp
  parser.cpp
  parser_expr_grammar.cpp
  parser_integer_grammar.cpp
  stats.cpp
  module.cpp
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
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
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
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  )
endif()
