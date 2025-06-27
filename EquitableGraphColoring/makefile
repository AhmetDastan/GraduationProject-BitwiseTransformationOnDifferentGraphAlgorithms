all: compile run clean

compile: BITS.cpp bitwise.cpp
	@g++ -O3 BITS.cpp -o bitsExe
	@g++ -O3 bitwise.cpp -o bitwiseExe

run:
	./bitsExe
	./bitwiseExe

clean:
	@rm -f *.o
	@rm -f bitsExe
	@rm -f bitwiseExe
