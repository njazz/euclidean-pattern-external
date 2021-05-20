mkdir builds
cd builds
mkdir win-mingw
cd win-mingw
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.mingw32.cmake
make && make install
