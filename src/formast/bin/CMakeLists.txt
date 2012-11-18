add_executable(formastapp
  main.cpp
)
set_target_properties(formastapp
  PROPERTIES OUTPUT_NAME formast)
target_link_libraries(formastapp formastlib)
install(
    TARGETS formastapp
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)
