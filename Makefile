

all: src/Parsers/FlowParser.cpp

src/Parsers/FlowParser.cpp: src/Parsers/FlowParser.y Makefile
	bison --defines=src/Parsers/FlowParser.h -o $@ src/Parsers/FlowParser.y

