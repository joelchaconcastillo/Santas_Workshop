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
  double best_score= SW->evaluate(x_var, daily_occupancy);
  vector<int> fam_perm;
  for(int i = 0; i < original.size(); i++) original.push_back(i);

  int N_training = 10;
  int max_size_feasible_solutions = 1000;
  while(true)
  { 
//     for(int ite = 0; ite < N_training; ite++)
//     {
//	//pickup an index famility combination...<--- fam_perm
//	random_shuffle(fam_perm.begin(), fam_perm.end());
//        //check the feasible solutions... <--- partial_solution
//        vector<vector<int>> feasible_subspace = branch_in_feasible_space(original, fam_perm, max_size_feasible_solutions);
//	bool improved = false;
//        for(int i = 0; i < feasible_subspace.size(); i++)
//        {	
//	   double score = SW->incremental_evaluation(original, fam_perm, feasible_subspace[i], daily_occupancy);
//  	   if(score < best_score)
//	   {
//		best_score = score;
//		best_partial_solution = feasible_subspace[i];
//		improved = true;
//	   }
// 	}
//     }

//     if(improved) //the solution is improved in the training part..
//     {
//         for(int i = 0 ; i < best_partial_solution.size(); i++)
//         {
//
//         }
//         improved = false;
//     }
  }
//  x_var = original;
//  fitness = score;
}
vector<int> branch_in_feasible_space(const vector<int> &original, const vector<int> fam_pem, const int max_size_feasible_solutions)
{
//  int size = 0;
//  while(size < max_size_feasible_solutions)
//  { 
//     if() //check feasibility...
//     {
//	size++;
//     }  
//  } 
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
