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
		  this->domain = SW_.domain;
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

		void subspace_local_search();
		void localSearch();
		void perturbe(vector<int> &x, int nvar);

 		pair< double, vector<int> >try_all_permutations(const vector<int> &perm, const int max_families, double preference_penalty, vector<int> daily_occupancy, vector< int > best_solution, double best_score);
		bool my_next_combination(vector<int> &row_perm, const vector<int> &upper_opt);
		double calculateFitness(vector<int> &x_ind);

 		//vector<vector<pair<int, int>>>  branch_in_feasible_space(const vector<int> &original, const vector<int> &fam_perm, const int max_size_feasible_solutions, vector<int> daily_occupancy );
		double incremental_evaluation();
		void print(vector<int> &sol);
		double fitness;

		SantaWorkshop *SW;
 		int dist;
		vector<int> x_var;
		vector<vector<int>> domain;
};

#endif
