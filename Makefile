.PHONY: test doxy clean
CXX = g++
CXXFLAGS = -Wall -lboost_program_options -lcrypto++ -lUnitTest++ -lboost_filesystem
TARGET = Utest
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(wildcard *.h)
test: $(TARGET) base.txt log.txt
doxy:
	doxygen
base.txt:
	@echo "login:password" >base.txt
log.txt:
	touch log.txt
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET) 

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
