# where we can find the built formast.so C++ library
BUILDPATH=../../../build/formast/

swig -c++ -python -I../include/ formast.i
g++ -fPIC -c formast_wrap.cxx -o formast_wrap.o -I/usr/include/python2.7 -I../include/
g++ -shared formast_wrap.o -L$BUILDPATH -lformast -o _formast.so
LD_LIBRARY_PATH=$BUILDPATH python test.py

