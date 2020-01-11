#include <signal.h>
#include <chrono> 
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;
using namespace std::chrono; 

double Individual::calculateFitness(vector<int> &x_ind){
  return SW->evaluate(x_ind);
}

void printBest(){
	
}

void Individual::subspace_local_search()
{
  vector<int> daily_occupancy(SW->N_DAYS, 0), fam_perm;
  
  
  double feasibility_level, accounting_penalty, preference_penalty = 0.0, fitness= SW->evaluate(x_var, daily_occupancy, preference_penalty, accounting_penalty, feasibility_level);
  for(int i = 0; i < x_var.size(); i++) fam_perm.push_back(i);
  int N_training = 100;
  int max_size_feasible_solutions = 2;

  vector<int> best_local_perm(max_size_feasible_solutions), best_local_option;
  cout << "current...." <<fitness<<endl;
  while(true)
  { 
     double best_local_score = fitness;
     vector<int> best_config(max_size_feasible_solutions);
    auto start = high_resolution_clock::now(); 
     for(int ite = 0; ite < N_training; ite++)
     {
	random_shuffle(fam_perm.begin(), fam_perm.end());
         double score = try_all_permutations(fam_perm, max_size_feasible_solutions, preference_penalty, accounting_penalty, daily_occupancy, x_var, fitness, best_config, feasibility_level);
        if(score < best_local_score)
	{
	   best_local_score= score;
	   best_local_option = best_config;
	   copy_n(fam_perm.begin(), max_size_feasible_solutions, best_local_perm.begin());
	}	

     }

       auto stop = high_resolution_clock::now(); 

       auto duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Time taken by function: "<< duration.count()/1.0e6 << " seconds" << endl; 


         if( best_local_score < fitness) 
	{
	   for(int i = 0 ; i < best_local_option.size(); i++)
	   {
	     if(best_local_option[i] == NOT_CHECK)continue;
	     x_var[best_local_perm[i]] = domain[best_local_perm[i]][best_local_option[i]];
	   }
	cout <<feasibility_level <<endl;
   	   fitness= SW->evaluate(x_var, daily_occupancy, preference_penalty, accounting_penalty, feasibility_level);
   	  //	cout <<"-- " << fitness<< " | "<< SW->evaluate(x_var) <<endl;;
//   	  	print(x_var);
   	  	cout <<"-- " << fitness<<endl;
	}
//	return;
  }
}
bool Individual::my_next_combination(vector<int> &row_perm, const vector<int> &upper_opt)
{
      int cont_nines = 0;
      row_perm[0]++;     
      for(int col = 0; col < row_perm.size()-1; col++) //check all cols in range..
      {
	if(row_perm[col] > upper_opt[col])
	{
	 if(row_perm[col] >= upper_opt[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}
      }
      if(row_perm[row_perm.size()-1] >= upper_opt[row_perm.size()-1]) cont_nines++;
   if(cont_nines == row_perm.size()) return false;
   return true;
}
double Individual::try_all_permutations(const vector<int> &perm, const int max_families, double preference_penalty, double accounting_penalty, vector<int> daily_occupancy, vector< int > best_solution, double best_score, vector<int> &best_partial_solution, double feasibility_level)
{
   vector<int> row_perm(max_families, NOT_CHECK), upper_opt(max_families, 9);
   while(my_next_combination(row_perm, upper_opt) )
   {
     //check first feasibility and then score..
     //double current_score = SW->incremental_evaluation(best_solution, row_perm, perm, preference_penalty, accounting_penalty, daily_occupancy, feasibility_level);
     double current_score = SW->incremental_evaluation(best_solution, row_perm, perm, preference_penalty, daily_occupancy);
     //uptade the best...
     if(current_score < best_score)
     {
        best_score = current_score;	
        best_partial_solution = row_perm;
     }
   }
 return best_score;
}
// vector<vector<pair<int, int>>>  Individual::branch_in_feasible_space(const vector<int> &original, const vector<int> &fam_perm, const int max_size_feasible_solutions, vector<int> daily_occupancy )
//{
//
//  vector< vector<pair<int, int > > > feasible_subspace;
//   int max_subspace_size = 50;
//   vector<int> row_perm(max_subspace_size, NOT_CHECK);
//   vector<int> branch_1(max_subspace_size, 9); // limit the feasible space of each permutation if it is necessary, inclusive the digit given..
//   int cont_nines = 0;
//   int tries = 0, maxtries = 1e5;
////   while(cont_nines < max_subspace_size)
//   while(feasible_subspace.size() < max_size_feasible_solutions && tries < maxtries)
//   {
//      cont_nines = 0 ;
//      tries++;
//      bool feasibility = true;     
//      vector<pair<int, int>> sparse_row;
//      vector<int> daily_occupancy2 = daily_occupancy;
//      for(int i = 0; i < row_perm.size(); i++)
//      {
//        if(row_perm[i] == NOT_CHECK) continue;
//	//check feasibility.. just accept this permutation if its a feasible solution...
//        int id_fam = fam_perm[i];
//        int day_in = domain[id_fam][row_perm[i]];
//        int day_out = original[id_fam];
//
//        daily_occupancy2[day_in] += SW->familiy_size[id_fam];
//        daily_occupancy2[day_out] -= SW->familiy_size[id_fam];
//
//	if(daily_occupancy2[day_in] > SW->MAX_OCCUPANCY) feasibility = false;
//	if(daily_occupancy2[day_in] < SW->MIN_OCCUPANCY) feasibility = false;
//	if(daily_occupancy2[day_out] > SW->MAX_OCCUPANCY) feasibility = false;
//	if(daily_occupancy2[day_out] < SW->MIN_OCCUPANCY) feasibility = false;
//
//
//       if(!feasibility) break;
//	 sparse_row.push_back(make_pair(id_fam, day_in));
//      }
//
//      
//   }  
////  cout << tries << " "<<feasible_subspace.size() <<endl;
//  return feasible_subspace;
//}
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
	x_var[i] = (rand()%100);
   }
}
double Individual::incremental_evaluation()
{
  return 0;
}
