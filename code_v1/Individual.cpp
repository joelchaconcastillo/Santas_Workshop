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
  for(int i = 0; i < original.size(); i++) fam_perm.push_back(i);

  int N_training = 10;
  int max_size_feasible_solutions = 100;
  while(true)
  { 
     for(int ite = 0; ite < N_training; ite++)
     {
	//pickup an index famility combination...<--- fam_perm
	random_shuffle(fam_perm.begin(), fam_perm.end());
        //check the feasible solutions... <--- partial_solution
        vector<vector<pair<int, int>>> feasible_subspace = branch_in_feasible_space(original, fam_perm, max_size_feasible_solutions, daily_occupancy);
	bool improved = false;
        for(int i = 0; i < feasible_subspace.size(); i++)
        {	
	   double score = SW->incremental_evaluation(original, feasible_subspace[i], daily_occupancy);
	  printf("%lf\n", score);
  	  // if(score < best_score)
	  // {
	  //      best_score = score;
	  //      //best_partial_solution = feasible_subspace[i];
	  //      improved = true;
	  // }
 	}
     }

 //    if(improved) //the solution is improved in the training part..
 //    {
 //        for(int i = 0 ; i < best_partial_solution.size(); i++)
 //        {

 //        }
 //        improved = false;
 //    }
  }
//  x_var = original;
//  fitness = score;
}
 vector<vector<pair<int, int>>>  Individual::branch_in_feasible_space(const vector<int> &original, const vector<int> fam_perm, const int max_size_feasible_solutions, vector<int> &daily_occupancy )
{

  vector< vector<pair<int, int > > > feasible_subspace;
   int max_subspace_size = 50;
   vector<int> row_perm(max_subspace_size, NOT_CHECK);
   vector<int> branch_1(max_subspace_size, 9); // limit the feasible space of each permutation if it is necessary
   int cont_nines = 0;
   int tries = 0;
//   while(cont_nines < max_subspace_size)
   while(feasible_subspace.size() < max_size_feasible_solutions)
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
        int current_occupancy_in = daily_occupancy[day_in] + SW->familiy_size[id_fam];
        int current_occupancy_out = daily_occupancy[day_out] - SW->familiy_size[id_fam];

       if(SW->MAX_OCCUPANCY < current_occupancy_in) feasibility=false;
       if( SW->MIN_OCCUPANCY > current_occupancy_in) feasibility=false;
       if(SW->MAX_OCCUPANCY < current_occupancy_out) feasibility=false;
       if( SW->MIN_OCCUPANCY > current_occupancy_out) feasibility=false;

       if(!feasibility) break;
	 sparse_row.push_back(make_pair(id_fam, day_in));
      }

      if( feasibility)
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
  cout << tries << " "<<feasible_subspace.size() <<endl;
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
