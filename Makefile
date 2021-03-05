BENCH := -I../benchmark/include/ -L../benchmark/build/src -L../benchmark/build/lib -lbenchmark -pthread
LI2 := -L/usr/lib/x86_64-linux-gnu/gap/ -Wl,-rpath,/usr/lib/x86_64-linux-gnu/gap/
CARLOPTS := -I../carl/src/ -I../carl/build/resources/include -L../carl/build/ -Wl,-rpath,../carl/build/ -lcarl-model -lcarl -lcarl-io
LPOPTS := -L../libpoly/build/src/ -Wl,-rpath,../libpoly/build/src/ -lpolyxx -lpoly
PARIOPTS := -lpari
OPTS := $(CARLOPTS) $(LPOPTS) $(BENCH) -lgmpxx -lgmp -Wall

main: main.cpp wrapper_carl.h wrapper_libpoly.h polynomial_generator.h test_functions.h
	g++ -O0 -g -std=c++17 $< -o $@ $(OPTS) 