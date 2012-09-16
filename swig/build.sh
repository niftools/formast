swig -c++ -python formast.i
gcc -fPIC -c formast_wrap.cxx -o formast_wrap.o -I/usr/include/python2.7 -I../include/
g++ -shared formast_wrap.o ../../../build/formast/libformast.a -o _formast.so
