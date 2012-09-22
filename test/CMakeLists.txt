foreach(TEST
        header_test
        expr_parse_xml_test
        )
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} formast-lib)
    add_test(format::${TEST} ${TEST})
endforeach()
