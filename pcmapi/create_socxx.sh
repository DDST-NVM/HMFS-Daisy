#/bin/bash

g++ -fPIC -c p_mmap.cpp
g++ -shared -o scmcxx.so p_mmap.o
