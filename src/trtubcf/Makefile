objects=main.o Trtubcf.o ../Ubcf.o ../tubcf/Tubcf.o ../trubcf/Trubcf.o 

main: ${objects} 
	g++ -o main ${objects} 

main.o : main.cpp 
	g++ -c main.cpp 

Trtubcf.o: Trtubcf.cpp ../Ubcf.cpp
	g++ -c Trtubcf.cpp 

Ubcf.o: ../Ubcf.cpp 
	g++ -c ../Ubcf.cpp

Tubcf.o: ../tubcf/Tubcf.cpp ../Ubcf.cpp
	g++ -c ../tubcf/Tubcf.cpp

Trubcf.o: ../trubcf/Trubcf.cpp ../Ubcf.cpp
	g++ -c ../trubcf/Trubcf.cpp

clean:
	rm main ${objects} 
