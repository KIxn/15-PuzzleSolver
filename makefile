main: main.o
	g++ -Wall -pedantic -g -o main main.o

main.o: main.cpp
	g++ -Wall -pedantic -g -c main.cpp

clean:
	rm *.o
	rm main
