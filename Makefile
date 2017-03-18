DIR=$(CURDIR)

CXXFLAGS=-g -Werror --std=c++11 -I src -I flow -DYYDEBUG=1 -DFLOW_CLICOLORS 

ARDUINO_BUILDER=cd /Applications/Arduino.app/Contents/Java && \
	./arduino-builder -verbose -hardware ./hardware -tools ./hardware/tools/avr -tools ./tools-builder -libraries ./libraries -fqbn arduino:avr:uno

SRC=src/Flow.cpp src/Parsers/FlowLexer.cpp src/Parsers/FlowParser.cpp src/DOM/Node.cpp src/DOM/Functions.cpp src/WebServer/WebServer.cpp
HEADERS=src/Flow.h src/Parsers/FlowLexer.h src/DOM/Node.h src/DOM/Functions.h src/DOM/Primitives.h src/WebServer/WebServer.h

FLOW_SRC=flow/main.cpp flow/Arduino.cpp flow/EEPROM.cpp flow/WiFi.cpp
FLOW_HEADERS=flow/Arduino.h flow/EEPROM.h flow/WiFi.h

TEST_SRC=test/main.cpp flow/Arduino.cpp flow/EEPROM.cpp flow/WiFi.cpp \
	test/ExpressionTests.cpp \
	test/SaveTests.cpp \
	test/FunctionTests.cpp
TEST_HEADERS=flow/Arduino.h flow/EEPROM.h test/CppUnitTest.h

WEB_SRC=web/webpack.config.js web/tsconfig.json \
	web/src/Flow.ts \
	web/src/components/Device.tsx \
	web/src/components/Hello.tsx

EXAMPLES=examples/Blink/Blink.ino

all: $(SRC) bin/flow bin/test $(EXAMPLES) docs/device.js docs/index.js

test: bin/test
	bin/test

bin/flow: $(SRC) $(FLOW_SRC) $(HEADERS) $(FLOW_HEADERS)
	mkdir -p bin
	g++ $(CXXFLAGS) $(SRC) $(FLOW_SRC) -o $@

bin/test: $(SRC) $(TEST_SRC) $(HEADERS) $(TEST_HEADERS)
	mkdir -p bin
	g++ $(CXXFLAGS) -I test $(SRC) $(TEST_SRC) -o $@

src/Parsers/FlowParser.cpp: src/Parsers/FlowParser.y
	bison -Werror --report=all --defines=src/Parsers/FlowParser.h -o $@ src/Parsers/FlowParser.y
	sed -i.bak 's/\[\] \=/\[\] PROGMEM =/g' $@

docs/device.js: web/src/device.tsx $(WEB_SRC)
	cd web && webpack

examples/Blink/Blink.ino: $(SRC)
	$(ARDUINO_BUILDER) -libraries $(DIR)/.. $(DIR)/$@
	touch $@
