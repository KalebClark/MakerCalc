CPPFLAGS := -I../MakerCalc -I.
CXXFLAGS := -g

.PHONY: all
all: testCalculator

testCalculator: calculator.o test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

calculator.o: ../MakerCalc/calculator.h ../MakerCalc/calculator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ ../MakerCalc/calculator.cpp

test.o: ../MakerCalc/calculator.h test.cpp