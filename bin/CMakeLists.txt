add_executable(formastapp
  ${formast_SOURCE_DIR}/src/formast/bin/main.cpp
)
set_target_properties(formastapp
  PROPERTIES OUTPUT_NAME formast)
target_link_libraries(formastapp formastlib)
install(
    TARGETS formastapp
    RUNTIME DESTINATION bin
    COMPONENT applications
)
