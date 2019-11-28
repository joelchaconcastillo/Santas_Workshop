#ifndef __TIME_TABLING_PROBLEM_H__
#define __TIME_TABLING_PROBLEM_H__
#include <bits/stdc++.h>
using namespace std;
#define FOREACH(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)

#define HARD 1
#define SOFT 2

void printBest();

class SantaWorkshop{
	public:
             
		SantaWorkshop(string file);
		~SantaWorkshop(){
		}
		double evaluate(vector<int> &x);
		void load(string file);
		void load_example(string file, vector<int> &x);
   		int N_FAM= 5000, N_OPTIONS=10, MAX_OCCUPANCY = 300, MIN_OCCUPANCY = 125, N_DAYS=100; 
		vector<vector<int>> domain, inv_domain;
		vector<int> familiy_size;
		
};
#endif
