main: MA.o SantaWorkshop.o Individual.o utils.o main.cpp 
	g++ -O3 -o $@ main.cpp MA.o SantaWorkshop.o Individual.o utils.o -std=c++11 -fopenmp

SantaWorkshop.o: SantaWorkshop.h SantaWorkshop.cpp
	g++ -O3 -c -o $@ SantaWorkshop.cpp -std=c++11  -fopenmp

Individual.o: Individual.h Individual.cpp
	g++ -O3 -c -o $@ Individual.cpp -std=c++11 -fopenmp

MA.o: MA.h MA.cpp
	g++ -O3 -c -o $@ MA.cpp -std=c++11 -fopenmp

utils.o: utils.cpp utils.h
	g++ -O3 -c -o $@ utils.cpp -std=c++11 -fopenmp

clean:
	rm -f main SantaWorkshop.o MA.o utils.o Individual.o
