
SRC=src/Flow.cpp src/Parsers/FlowLexer.cpp src/Parsers/FlowParser.cpp
HEADERS=src/Flow.h src/Parsers/FlowLexer.h

FLOW_SRC=flow/main.cpp flow/Arduino.cpp
FLOW_HEADERS=flow/Arduino.h


all: $(SRC) bin/flow

bin/flow: $(SRC) $(FLOW_SRC) $(HEADERS) $(FLOW_HEADERS) Makefile
	mkdir -p bin
	g++ -g -I src -I flow -DYYDEBUG=1 -DFLOW_CLICOLORS $(SRC) $(FLOW_SRC) -o $@

src/Parsers/FlowParser.cpp: src/Parsers/FlowParser.y Makefile
	bison --report=all --defines=src/Parsers/FlowParser.h -o $@ src/Parsers/FlowParser.y
	sed -i.bak 's/\[\] \=/\[\] PROGMEM =/g' $@

