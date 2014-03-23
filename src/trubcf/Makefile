main: main.o Trubcf.o ../Ubcf.o  
	g++ -o main main.o Trubcf.o ../Ubcf.o 

main.o : main.cpp 
	g++ -c main.cpp 

Trubcf.o: Trubcf.cpp ../Ubcf.cpp
	g++ -c Trubcf.cpp 

Ubcf.o: ../Ubcf.cpp 
	g++ -c ../Ubcf.cpp

clean:
	rm main main.o Trubcf.o ../Ubcf.o
