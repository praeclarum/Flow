
SRC=src/Flow.cpp src/Parsers/FlowParser.cpp

FLOW_SRC=flow/main.cpp flow/Arduino.cpp


all: $(SRC) bin/flow

bin/flow: $(SRC) $(FLOW_SRC) Makefile
	mkdir -p bin
	g++ -g -I src -I flow $(SRC) $(FLOW_SRC) -o $@

src/Parsers/FlowParser.cpp: src/Parsers/FlowParser.y Makefile
	bison --defines=src/Parsers/FlowParser.h -o $@ src/Parsers/FlowParser.y
	sed -i.bak 's/\[\] \=/\[\] PROGMEM =/g' $@

