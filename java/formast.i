%include "../swig/formast.i"

// java specific modifications

// automatically load the module
// see http://stackoverflow.com/questions/8168517/generating-java-interface-with-swig
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("formast");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }
%}
