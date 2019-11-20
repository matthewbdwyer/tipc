rm -rf build/
mkdir build
cd build
cmake ../src
make
./tipc -p $1
cd ..