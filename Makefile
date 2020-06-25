BENCH := -I../benchmark/include/ -L../benchmark/build/src -L../benchmark/build/lib -lbenchmark -pthread
LI2 := -L/usr/lib/x86_64-linux-gnu/gap/ -Wl,-rpath,/usr/lib/x86_64-linux-gnu/gap/
CARLOPTS := -I../carl/src/ -I../carl/build/resources/include -L../carl/build/ -Wl,-rpath,../carl/build/ -lcarl-model -lcarl -lcarl-io
LPOPTS := -L../libpoly/build/src/ -Wl,-rpath,/home/gereon/stuff/libpoly/build/src/ -lpoly
PARIOPTS := -lpari
OPTS := $(CARLOPTS) $(LPOPTS) $(PARIOPTS) $(BENCH) -lgmpxx -lgmp

main: main.cpp wrapper_carl.h wrapper_lp.h wrapper_pari.h
	g++ $< -o $@ -O0 -g -std=c++17 $(OPTS)
