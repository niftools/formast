# extra include for printer.hpp
include_directories(${FORMAST_SOURCE_DIR}/src/app)

foreach(TEST
        header_test
        expr_parse_xml_test
        )
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} formastlib)
    add_test(format::${TEST} ${TEST})
endforeach()
