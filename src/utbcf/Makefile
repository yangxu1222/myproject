main: main.o UTbcf.o ../Ubcf.o  
	g++ -o main main.o UTbcf.o ../Ubcf.o 

main.o : main.cpp 
	g++ -c main.cpp 

UTbcf.o: UTbcf.cpp ../Ubcf.cpp
	g++ -c UTbcf.cpp 

Ubcf.o: ../Ubcf.cpp 
	g++ -c ../Ubcf.cpp

clean:
	rm utbcf $(objects)
