CXXFLAGS=-std=c++11
TARGET=test
OBJFILES=$(wildcard *.o)
WIN_EXEFILES=$(wildcard *.exe)
UNIX_EXEFILES=$(TARGET)
test: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	$(CXX) $^ -o $(TARGET)

.PHONY: clean
clean:
	rm $(OBJFILES)
	rm $(WIN_EXEFILES)
	rm $(UNIX_EXEFILES)
