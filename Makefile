main: main.o MyVector.hpp
	g++ -o main main.o

tests: tests.o MyVector.hpp
	g++ -o tests tests.o

main.o: main.cpp

tests.o: tests.cpp

clean:
	rm -f *.o
