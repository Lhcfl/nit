cmake -S . -B build
cmake --build build

cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..