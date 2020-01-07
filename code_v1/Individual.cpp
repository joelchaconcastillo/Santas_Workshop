#include <signal.h>
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;

double Individual::calculateFitness(vector<int> &x_ind){
  return SW->evaluate(x_ind);
}

void printBest(){
	
}

void Individual::subspace_local_search()
{
  vector<int> original  = x_var;
  vector<int> daily_occupancy(SW->N_DAYS, 0);
  vector<pair<int, int>> best_partial_solution;
  double preference_penalty = 0.0;
  double best_score= SW->evaluate(x_var, daily_occupancy, preference_penalty);
  cout <<best_score<<endl;
  vector<int> fam_perm;
  for(int i = 0; i < original.size(); i++) fam_perm.push_back(i);

  int N_training = 1000;
  int max_size_feasible_solutions = 1000;
  bool improved = false;
  while(true)
  { 
     for(int ite = 0; ite < N_training; ite++)
     {
	//pickup an index famility combination...<--- fam_perm
	random_shuffle(fam_perm.begin(), fam_perm.end());
        //check the feasible solutions... <--- partial_solution
        vector<vector<pair<int, int>>> feasible_subspace = branch_in_feasible_space(original, fam_perm, max_size_feasible_solutions, daily_occupancy);
//	for(int i = 0 ; i < 10; i++) cout << fam_perm[i] << " ";
//	cout <<endl;
//	for(int i = 0 ; i < feasible_subspace.size(); i++)
//	{
//	for(int j = 0 ; j < feasible_subspace[i].size(); j++)
//		 cout << feasible_subspace[i][j].second << " ";
//	cout <<endl;
//	}
        for(int i = 0; i < feasible_subspace.size(); i++)
        {	
	   double score = SW->incremental_evaluation(preference_penalty, original, feasible_subspace[i], daily_occupancy);
	   
  	   if(score < best_score)
	   {
	        best_score = score;
	        best_partial_solution = feasible_subspace[i];
	        improved = true;
		cout << best_score <<endl;
	   }
 	} 
     }
//	cout << best_score <<endl;
     if(improved) //the solution is improved in the training part..
     {
         for(int i = 0 ; i < best_partial_solution.size(); i++)
	   original[best_partial_solution[i].first] = best_partial_solution[i].second;	    
  	 best_score= SW->evaluate(original, daily_occupancy, preference_penalty);
         
	cout <<"-- " << best_score <<endl;
	print(original);
         improved = false;
     }
  }
//  x_var = original;
//  fitness = score;
}
 vector<vector<pair<int, int>>>  Individual::branch_in_feasible_space(const vector<int> &original, const vector<int> &fam_perm, const int max_size_feasible_solutions, vector<int> daily_occupancy )
{

  vector< vector<pair<int, int > > > feasible_subspace;
   int max_subspace_size = 50;
   vector<int> row_perm(max_subspace_size, NOT_CHECK);
   vector<int> branch_1(max_subspace_size, 3); // limit the feasible space of each permutation if it is necessary, inclusive the digit given..
   int cont_nines = 0;
   int tries = 0, maxtries = 1e4;
//   while(cont_nines < max_subspace_size)
   while(feasible_subspace.size() < max_size_feasible_solutions && tries < maxtries)
   {
      cont_nines = 0 ;
      tries++;
      bool feasibility = true;     
      vector<pair<int, int>> sparse_row;
      for(int i = 0; i < row_perm.size(); i++)
      {
        if(row_perm[i] == NOT_CHECK) continue;
	//check feasibility.. just accept this permutation if its a feasible solution...
        int id_fam = fam_perm[i];
        int day_in = domain[id_fam][row_perm[i]];
        int day_out = original[id_fam];

        daily_occupancy[day_in] += SW->familiy_size[id_fam];
        daily_occupancy[day_out] -= SW->familiy_size[id_fam];

	if(daily_occupancy[day_in] > SW->MAX_OCCUPANCY) feasibility = false;
	if(daily_occupancy[day_in] < SW->MIN_OCCUPANCY) feasibility = false;
	if(daily_occupancy[day_out] > SW->MAX_OCCUPANCY) feasibility = false;
	if(daily_occupancy[day_out] < SW->MIN_OCCUPANCY) feasibility = false;


       if(!feasibility) break;
	 sparse_row.push_back(make_pair(id_fam, day_in));
      }

      if( feasibility && !sparse_row.empty())
      feasible_subspace.push_back(sparse_row); 

      row_perm[0]++;     
      for(int col = 0; col < row_perm.size()-1; col++) //check all cols in range..
      {
	if(row_perm[col] > branch_1[col])
	{
	 if(row_perm[col] >= branch_1[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}
      }
      if(row_perm[max_subspace_size-1] > branch_1[max_subspace_size-1]) cont_nines++;
   }  
//  cout << tries << " "<<feasible_subspace.size() <<endl;
  return feasible_subspace;
}
void Individual::localSearch()
{

}
int Individual::getDistance(Individual &ind){
return 0;
}
void Individual::Mutation(double pm){
}
void Individual::Crossover(Individual &ind){
}
void Individual::print(vector<int> &sol){
  cout << "family_id,assigned_day"<<endl;
  for(int i = 0; i < sol.size(); i++)
  {
    cout <<i <<","<<sol[i]+1<<endl; 
  }
}
void Individual::initialization()
{
   x_var.resize(domain.size(), 0);
   for(int i = 0; i < domain.size(); i++)
   {
	x_var[i] = (rand()%100)+1;
   }
}
double Individual::incremental_evaluation()
{
  return 0;
}
