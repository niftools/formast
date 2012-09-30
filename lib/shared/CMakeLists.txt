add_library(formastlib SHARED
  ${FORMAST_LIBRARY_SOURCES}
)

set_target_properties(formastlib
  PROPERTIES OUTPUT_NAME formast
)
