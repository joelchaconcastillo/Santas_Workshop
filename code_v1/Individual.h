#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__
#include <bits/stdc++.h>
using namespace std;
#define FOREACH(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)


void printBest();

class Individual{
	public:
		
		Individual(SantaWorkshop &SW_){
		  this->SW= &SW_;
		  //initialization....
		  initialization();
		  //Load example solution to check the evaluator...
		}
		Individual(){}
		~Individual(){
		}
		void initialization();
		int getDistance(Individual &ind);
		void Mutation(double pm);
		void Crossover(Individual &ind);

		void iterated_local_search();
		void localSearch();

		long long calculateFitness(vector<int> &x_ind);
		long long incremental_evaluation();
		void print();
		long long fitness;

		SantaWorkshop *SW;
 		int dist;
		vector<int> x_var;
		vector<vector<int>> domain;
};

#endif
