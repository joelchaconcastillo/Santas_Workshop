main: MA.o SantaWorkshop.o Individual.o utils.o main.cpp 
	g++ -O2 -o $@ main.cpp MA.o SantaWorkshop.o Individual.o utils.o -std=c++11

SantaWorkshop.o: SantaWorkshop.h SantaWorkshop.cpp
	g++ -O2 -c -o $@ SantaWorkshop.cpp -std=c++11

Individual.o: Individual.h Individual.cpp
	g++ -O2 -c -o $@ Individual.cpp -std=c++11

MA.o: MA.h MA.cpp
	g++ -O2 -c -o $@ MA.cpp -std=c++11

utils.o: utils.cpp utils.h
	g++ -O2 -c -o $@ utils.cpp -std=c++11

clean:
	rm -f main SantaWorkshop.o MA.o utils.o Individual.o
