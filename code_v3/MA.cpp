#include <sys/time.h>
#include <iostream>
#include <signal.h>
#include <omp.h>

#include "MA.h"
#include "utils.h"

using namespace std;

MA::MA(int N_, double pc_, double pm_, double finalTime_, SantaWorkshop &SW_){
	if (N_ % 2){ cerr << "El tam. de poblacion debe ser par" << endl; exit(-1); }
	this->SW = SW_;
	this->N = N_;
	this->pc = pc_;
	this->pm = pm_;
	this->finalTime = finalTime_;
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
        random_device r;
        
        for (int i = 0, N = omp_get_max_threads(); i < N; ++i) {
           generators.emplace_back(default_random_engine(r()));
        }
	this->initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
}

void MA::initPopulation(){
	for (int i = 0; i < N; i++){
		//cout << "Crea ind " << i << endl;
		Individual *ei = new Individual((this->SW));
		population.push_back(ei);	
	}
}
//Select parents with binary selection
void MA::selectParents(){
	parents.clear();
	for (int i = 0; i < N; i++){
		int first = getRandomInteger0_N(N - 1);
		int second = getRandomInteger0_N(N - 1);
		if (population[first]->fitness <= population[second]->fitness){
			parents.push_back(population[first]);
		} else {
			parents.push_back(population[second]);
		}
	}
}

void MA::crossover(){
	cout << "============ crossover " <<endl;
	cout << "before..."<<endl;
	for (int i = 0; i < parents.size(); i++){
		cout << i << ": "<< parents[i]->fitness <<endl;
		Individual *ei = new Individual((this->SW));
		*ei = *parents[i];
		offspring.push_back(ei);
	}
	cout << "after..."<<endl;
	for (int i = 0; i < offspring.size(); i+=2){
		if (generateRandomDouble0_Max(1) <= pc){
			offspring[i]->Crossover(*(offspring[i+1]));
		}
		offspring[i]->Evaluation();
		offspring[i+1]->Evaluation();
		cout << i << ": "<< offspring[i]->fitness <<endl;
		cout << i+1 << ": "<< offspring[i+1]->fitness <<endl;
	}
}

void MA::mutation(){
	 #pragma omp parallel for
	for (int i = 0; i < offspring.size(); i++){
		offspring[i]->Mutation(pm);
	}
}

void MA::localSearch(){

//	for (int i = 0; i < offspring.size(); i++) cout << i << ": "<<offspring[i]->fitness <<endl;
	cout << "localsearch........"<<endl;
	 #pragma omp parallel for
	for (int i = 0; i < offspring.size(); i++){
		cout << i << ": "<<offspring[i]->fitness <<endl;
		  offspring[i]->localSearch(generators[omp_get_thread_num()]);
		cout << i << ": "<<offspring[i]->fitness <<endl;
	}

//	for (int i = 0; i < offspring.size(); i++) cout << i << ": "<<offspring[i]->fitness <<endl;
}


void MA::replacement(){
	vector < Individual* > all;
	
	//Join population and offspring
	for (int i = 0; i < population.size(); i++){
		if(bestI.fitness > population[i]->fitness) bestI = *(population[i]);

		all.push_back(population[i]);
		all.back()->dist = INT_MAX;
	}
	population.clear();
	for (int i = 0; i < offspring.size(); i++){
		if(bestI.fitness > offspring[i]->fitness) bestI = *(offspring[i]);
		all.push_back(offspring[i]);
		all.back()->dist = INT_MAX;
	}
	offspring.clear();
	
	//Select best solution
	int indexBest = 0;
	for (int i = 1; i < all.size(); i++){
		if (all[i]->fitness < all[indexBest]->fitness){
			indexBest = i;
		}
	}
	population.push_back(all[indexBest]);
	all[indexBest] = all.back();
	all.pop_back();

	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	elapsedTime -= initialTime;

	//Select next N - 1 solution
	double D = DI - DI * elapsedTime / finalTime;
	//cout <<D <<endl;
	//cout << "Distancia requerida: " << D << endl;
	while(population.size() != N){
		//Update distances
		for (int i = 0; i < all.size(); i++){
			all[i]->dist = min(all[i]->dist, all[i]->getDistance(*(population.back())));
		}
		//Select best option
		indexBest = 0;
		for (int i = 1; i < all.size(); i++){
			bool betterInDist =	(all[i]->dist > all[indexBest]->dist);
			bool eqInDist = (all[i]->dist == all[indexBest]->dist);
			bool betterInFit = (all[i]->fitness < all[indexBest]->fitness);
			bool eqInFit = (all[i]->fitness == all[indexBest]->fitness);
			if (all[indexBest]->dist < D){//Do not fulfill distance requirement
				if ((betterInDist) || (eqInDist && betterInFit)){
					indexBest = i;
				}
			} else {
				if (all[i]->dist >= D){
					if ((betterInFit) || (eqInFit && betterInDist)){
						indexBest = i;
					}
				}
			}
		}
		//Insert best option
		population.push_back(all[indexBest]);
		all[indexBest] = all.back();
		all.pop_back();
	}
	//Release memory
	for (int i = 0; i < all.size(); i++){
		delete(all[i]);
	}
}

void MA::initDI(){
	double meanDistance = 0;
	for (int i = 0; i < population.size(); i++){
		for (int j = i + 1; j < population.size(); j++){
			meanDistance += population[i]->getDistance(*(population[j]));
			//cout << "Distancia: " << population[i]->ind.getDistance(population[j]->ind) << endl;
		}
	}
	meanDistance /= (population.size() * (population.size() - 1)) / 2;
	DI = meanDistance * 1;//TODO: Check
}
void MA::run(){
	initPopulation();
	evaluation(population);
	initDI();
	bestI = *(population[0]);
	int generation = 0;
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	elapsedTime -= initialTime;
	while( elapsedTime < finalTime  ){//Infinitas generaciones
	
		int minDistance = INT_MAX;
		for (int i = 0; i < population.size(); i++){
			for (int j = i + 1; j < population.size(); j++){
				minDistance = min(minDistance, population[i]->getDistance(*(population[j])));
			}
		}
	//	cout << "Distancia: " << minDistance <<endl;
		cout << "========================" <<endl;
		cout << "Generacion " << generation << endl;
		selectParents();
		crossover();
		//mutation();
		evaluation(offspring);
		
		localSearch();
		replacement();
		generation++;
	       printBest();
	    gettimeofday(&currentTime, NULL);
	    elapsedTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	    elapsedTime -= initialTime;
	    cout << "remaining time... "<< finalTime - elapsedTime <<endl;

	}
	bestI.print();
}

void MA::printBest()
{
   double best_fitness = DBL_MAX;
   for(int i = 0 ; i < population.size(); i++)
   {
	if(best_fitness > population[i]->fitness)
	{
	  best_fitness = population[i]->fitness;
	}
   }
	cout << best_fitness <<endl;
}
void MA::evaluation(vector< Individual * > &pool)
{
   #pragma omp parallel for
   for(int i = 0 ; i  < pool.size(); i++)
   { 
	pool[i]->Evaluation();
   }
}
