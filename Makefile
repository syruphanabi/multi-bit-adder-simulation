CXX=g++-7
CFLAGS=-g -std=c++11
OMPFLAGS = -fopenmp

#clang++ -g -std=c++11 -o out SimObj.cpp INPUT.cpp GNode.cpp Graph.cpp Event.cpp main.cpp
out: *.cpp
	$(CXX) $(CFLAGS) $(OMPFLAGS) -o $@ $^

clean:
	rm *.o out
