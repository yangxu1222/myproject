main: main.o Ubcf.o   
	g++ -o main main.o Ubcf.o 

main.o : main.cpp
	g++ -c main.cpp

Ubcf.o: Ubcf.cpp 
	g++ -c Ubcf.cpp

clean:
	rm main main.o Ubcf.o
