#ifndef __TIME_TABLING_PROBLEM_H__
#define __TIME_TABLING_PROBLEM_H__
#include <bits/stdc++.h>
using namespace std;
#define FOREACH(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)

#define HARD 1
#define SOFT 2
#define NOT_OPTION -1
#define NOT_CHECK -1

void printBest();
struct Solution
{
 	   double score, preference_penalty, accounting_penalty, unfeasibility_score;
	   bool feasible;
 	   vector<int> x, daily_occupancy;
};
class SantaWorkshop{
	public:
		
		SantaWorkshop(string file);
		~SantaWorkshop(){
		}
		void evaluate(struct Solution &S);
		double evaluate(vector<int> &x);
//		double evaluate(struct Solution &S);

		double Accounting_Penalty_Computation(vector<int> &daily_occupancy);

		double incremental_evaluation_unfeasible(struct Solution &S, const vector<int> &row_perm, const vector<int> &perm);

		double incremental_evaluation(struct Solution &S, const vector<int> &row_perm, const vector<int> &perm);
		double incremental_evaluation_fast(struct Solution &S, const vector<int> &row_perm, const vector<int> &perm);


		void init_table_permutations(int max_subspace_size);


		void load(string file);
 	        inline double quality_unfeasibility(int sum_occupancy){  return max(0.0, 1e10*(sum_occupancy-MAX_OCCUPANCY)) + max(0.0, 1e10*(MIN_OCCUPANCY-sum_occupancy)); }
		void load_example(string file, vector<int> &x);
   		int N_FAM= 5000, N_OPTIONS=10, MAX_OCCUPANCY = 300, MIN_OCCUPANCY = 125, N_DAYS=100, max_occupancy, min_occupancy; 
		vector<vector<int>> domain, inv_domain;
		vector<int> familiy_size;
		vector<double> c1, c2, c3;
	        vector< vector<double> > preference_costs, accounting_costs;
		vector< vector<pair<int, int> > > table_permutations;
};
#endif
