# extra include for printer.hpp
include_directories(${formast_SOURCE_DIR}/src/formast/bin)

# header tests (need no linking)
foreach(TEST
        header_formast_test
        header_enum_stats_impl_test
        header_expr_impl_test
        header_stats_impl_test
        header_module_impl_test
        )
    add_executable(${TEST} ${TEST}.cpp)
    add_test(format::${TEST} ${TEST})
endforeach()

# other tests
foreach(TEST
        expr_parse_xml_test
        )
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} formastlib)
    add_test(format::${TEST} ${TEST})
endforeach()
