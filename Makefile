CPP_STD = c++0x

all: proc ass

.PHONY: proc
proc: 
	g++ -o proc -std=$(CPP_STD) Location.cpp State.cpp FetchUnit.cpp DecodeUnit.cpp ExecutionUnit.cpp common.cpp option.cpp main.cpp

.PHONY: ass
ass:
	g++ -o ass -std=$(CPP_STD) common.cpp option.cpp Assembler.cpp

clean:
	rm ass
	rm proc
	rm -rf *.o
	rm -rf a.out
