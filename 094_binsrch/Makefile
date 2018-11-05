test: bsearch.o test.o
	g++ -o test bsearch.o test.o
bsearch.o: search.cpp
	g++ -std=c++98 -pedantic -Wall -Werror -c -o bsearch.o search.cpp
test.o: test-bsearch.cpp
	g++ -std=c++98 -pedantic -Wall -Werror -c -o test.o test-bsearch.cpp
