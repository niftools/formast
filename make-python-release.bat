:: batch script to build a binary egg package for the currently active Python

cd ..
del /q /s build-formast\
mkdir build-formast
cd build-formast
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_JAVA=OFF ..\formast
nmake
cpack -G ZIP
cd swig\python\package
python setup.py bdist_egg

