add_library(formaststaticlib STATIC
  ${FORMAST_LIBRARY_SOURCES}
)

set_target_properties(formaststaticlib
  PROPERTIES
  OUTPUT_NAME formast
  COMPILE_FLAGS -DFORMAST_STATIC
)
