CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
sortLines: sortLines.o
	g++ -o sortLines $(CCFLAGS) sortLines.cpp
%.o: %.cpp
	g++ -c $(CCFLAGS) $<

clean: rm -f *.o *~ code
