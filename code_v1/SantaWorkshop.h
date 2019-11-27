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
		void Load(string file);

		
};
#endif
