main: main.cpp
	clang++ -std=c++23 main.cpp -o aoc

debug: main.cpp
	clang++ -std=c++23 -O0 -g main.cpp -o aoc-debug

asm: main.cpp
	clang++ -main=c++23 -O0 -S -mllvm --x86-asm-syntax=intel main.cpp