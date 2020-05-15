plagiarismCatcher: main.o cheaters.o
	g++ -std=c++0x main.o cheaters.o -o plagiarismCatcher
	
main.o: main.cpp
	g++ -std=c++0x -c main.cpp
	
cheaters.o: cheaters.cpp
	g++ -std=c++0x -c cheaters.cpp
	
	
clean:
	rm *.o plagiarismCatcher