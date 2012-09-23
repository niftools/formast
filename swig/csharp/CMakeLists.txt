set_source_files_properties(formast.i PROPERTIES CPLUSPLUS ON)
swig_add_module(formastcsharp csharp formast.i)
swig_link_libraries(formastcsharp formast-lib)
