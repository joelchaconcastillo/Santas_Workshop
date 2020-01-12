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

  struct Solution S;
  S.score = fitness;
  S.x = x_var;

  
  SW->evaluate(S);

  cout << "current...." <<S.score<<endl;

  vector<int> fam_perm;
  for(int i = 0; i < x_var.size(); i++) fam_perm.push_back(i);

  int N_training = 10;
  int subspace_size= 5;
  while(true)
  { 
     vector<int> best_local_perm_family(subspace_size), best_local_perm_days(subspace_size); //variables to find the local optimal..
     double best_local_score = (S.feasible)?S.score:S.unfeasibility_score;

     auto start = high_resolution_clock::now();  ///taking the computing time..
     for(int ite = 0; ite < N_training; ite++)
     {
	random_shuffle(fam_perm.begin(), fam_perm.end());
        try_all_permutations(S, fam_perm, best_local_score, best_local_perm_family, best_local_perm_days); //it replaces the best solution..
     }
       auto stop = high_resolution_clock::now(); 
       auto duration = duration_cast<microseconds>(stop - start); 
       //// 
//       cout << "Time taken by function: "<< duration.count()/1.0e6 << " seconds" << endl; 


        if( best_local_score < ((S.feasible)?S.score:S.unfeasibility_score)) 
	{
	   for(int i = 0 ; i < subspace_size; i++)
	   {
	     if(best_local_perm_days[i] == NOT_CHECK)continue;
	     S.x[best_local_perm_family[i]] = domain[best_local_perm_family[i]][best_local_perm_days[i]];
	   }
   	   SW->evaluate(S);
	cout << S.score<<endl;
//   	  	print(S.x_var);
	}

//	cout <<S.feasible <<" "<< ((S.feasible)?S.score:S.unfeasibility_score) << " " << best_local_score<< endl;
  }
  fitness = S.score;
  x_var = S.x;
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
void Individual::try_all_permutations(struct Solution &S, const vector<int> &perm, double &best_local_score, vector<int> &best_local_perm_family, vector<int> &best_local_perm_days)
{
   int k_subspace = best_local_perm_family.size();
   vector<int> row_perm(k_subspace, NOT_CHECK), upper_opt(k_subspace, 9); //it can be optimized................
   while(my_next_combination(row_perm, upper_opt) )
   {
     //check first feasibility and then score..
     double current_score = 0.0;
     if(S.feasible)
     {
       current_score = SW->incremental_evaluation(S, row_perm, perm);
     }
     else
     {
       current_score = SW->incremental_evaluation_unfeasible(S, row_perm, perm);
       if(current_score <= 0) //if this movement changes to feasible, then check the feasibility, yep yhe unfeasibility score should be major than the feasible score..
          current_score -= SW->incremental_evaluation(S, row_perm, perm);
     }
     if( best_local_score > current_score )
     {
	best_local_score = current_score;
	copy_n(perm.begin(), k_subspace, best_local_perm_family.begin());
	copy_n(row_perm.begin(), k_subspace , best_local_perm_days.begin());
     }
   }
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
	x_var[i] = (rand()%100);
   }
}
double Individual::incremental_evaluation()
{
  return 0;
}
