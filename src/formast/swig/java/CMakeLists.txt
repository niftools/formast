find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include_directories(${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS})
set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
set(JAVA_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
file(MAKE_DIRECTORY ${JAVA_SOURCE_DIRECTORY})
file(MAKE_DIRECTORY ${JAVA_BINARY_DIRECTORY})
set(CMAKE_SWIG_OUTDIR ${JAVA_SOURCE_DIRECTORY}/org/niftools/formast/)
set(CMAKE_SWIG_FLAGS -package "org.niftools.formast" ${CMAKE_SWIG_GLOBAL_FLAGS})
set_source_files_properties(formast.i PROPERTIES CPLUSPLUS ON)
swig_add_module(formastjava java formast.i)
swig_link_libraries(formastjava formastlib)

# target for jar
set(JAR_FILE formast-${VERSION}.jar)
set(JAR_SOURCE_FILE formast-source-${VERSION}.jar)
add_custom_target(formast-jar ALL DEPENDS ${JAR_FILE})
set(JAVA_SOURCE_CODE ${JAVA_SOURCE_DIRECTORY}/org/niftools/formast/*.java)

# generate jar
add_custom_command(
  OUTPUT ${JAR_FILE}
  COMMENT "Creating jar file..."
  COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BINARY_DIRECTORY} ${JAVA_SOURCE_CODE}
  COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/${JAR_FILE} -C ${JAVA_BINARY_DIRECTORY} org
  COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/${JAR_SOURCE_FILE} org
  DEPENDS formastjava
)
