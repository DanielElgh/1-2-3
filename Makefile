PROJECT := 123
SRC := $(wildcard *.cpp)
OBJ := $(SRC:%.cpp=%.o)
#CXX = g++-8

CXXFLAGS := -Ofast -march=native -mtune=native -std=c++11 -pthread
WARNFLAGS := -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual

$(PROJECT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(WARNFLAGS) -o $@ $<


.PHONY: all
all: $(PROJECT)

.PHONY: clean
clean:
	$(RM) $(PROJECT) $(OBJ) *.gcov *.gc??

.PHONY: test
test: $(PROJECT)
	./$(PROJECT)
