// python specific modifications

// exception handling (must come before ../formast.i)

%feature("director:except") {
    if ($error != NULL) {
        throw Swig::DirectorMethodException();
    }
}

%exception {
    try { $action }
    catch (Swig::DirectorException &) { SWIG_fail; }
    SWIG_CATCH_STDEXCEPT // catch std::exception
    catch (...) {
        SWIG_exception(SWIG_UnknownError, "Unknown exception");
    }
}

%include "../formast.i"
