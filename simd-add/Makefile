all: bench

bench: vec_add.cpp benchmark.cpp
	g++ -O3 -mavx2 -std=c++20 $^ -lbenchmark -pthread -o $@
