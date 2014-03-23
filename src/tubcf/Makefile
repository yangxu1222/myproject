main: main.o Tubcf.o ../Ubcf.o  
	g++ -o main main.o Tubcf.o ../Ubcf.o 

main.o : main.cpp 
	g++ -c main.cpp 

Tubcf.o: Tubcf.cpp ../Ubcf.cpp
	g++ -c Tubcf.cpp 

Ubcf.o: ../Ubcf.cpp 
	g++ -c ../Ubcf.cpp

clean:
	rm main main.o Tubcf.o ../Ubcf.o
