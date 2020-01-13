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

  int N_training = 1;
  int subspace_size= 1+rand()%6;
  int sub_domain_size = 1;
  while(true)
  { 
     vector<int> best_local_perm_family(subspace_size), best_local_perm_days(subspace_size); //variables to find the local optimal..
     double best_local_score = S.score;

     auto start = high_resolution_clock::now();  ///taking the computing time..
     for(int ite = 0; ite < N_training; ite++)
     {
	random_shuffle(fam_perm.begin(), fam_perm.end());
        try_all_permutations(S, fam_perm, best_local_score, best_local_perm_family, best_local_perm_days, sub_domain_size); //it replaces the best solution..
     }
       auto stop = high_resolution_clock::now(); 
       auto duration = duration_cast<microseconds>(stop - start); 
       //// 
//       cout << "Time taken by function: "<< duration.count()/1.0e6 << " seconds" << endl; 
        if( best_local_score < S.score) 
	{
	   
	   for(int i = 0 ; i < subspace_size; i++)
	   {
	     if(best_local_perm_days[i] == NOT_CHECK)continue;
	     S.x[best_local_perm_family[i]] = domain[best_local_perm_family[i]][best_local_perm_days[i]];
	   }
   	   SW->evaluate(S);
	printf("%.8f %.8f %.8f\n", best_local_score ,S.score, SW->evaluate(S.x));
//   	  	print(S.x_var);
	}

  }
  fitness = S.score;
  x_var = S.x;
}
bool Individual::my_next_combination(vector<int> &row_perm, const vector<int> &upper_opt)
{
      int cont_nines = 0;
      row_perm[0]++;     
      int size_w = row_perm.size()-1; 
      for(int col = 0; col < size_w; col++) //check all cols in range..
      {
	if(row_perm[col] > upper_opt[col])
	{
	 if(row_perm[col] >= upper_opt[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}
      }
      if(row_perm[size_w] >= upper_opt[size_w]) cont_nines++;
   if(cont_nines == size_w+1) return false;
   return true;
}
bool Individual::my_next_combination(vector<int> &row_perm, const vector<int> &upper_opt, const vector<int> &fam_perm, vector<pair<int, int> > &fam_day_perm, int &Real_size)
{
      int cont_nines = 0;
      Real_size  = 0;
      row_perm[0]++;     
      int size_w = row_perm.size()-1; 
      for(int col = 0; col < size_w; col++) //check all cols in range..
      {
	if(row_perm[col] > upper_opt[col])
	{
	 if(row_perm[col] >= upper_opt[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}
	if(row_perm[col] != NOT_CHECK)
	{
	 fam_day_perm[Real_size++] = make_pair(fam_perm[col], domain[fam_perm[col]][row_perm[col]]);
	}
      }
      if(row_perm[size_w] >= upper_opt[size_w]) cont_nines++;
      if(row_perm[size_w] != NOT_CHECK)
      {
	   fam_day_perm[Real_size++] = make_pair(fam_perm[size_w], domain[fam_perm[size_w]][row_perm[size_w]]);
      }
   if(cont_nines == size_w+1) return false;
   return true;
}
void Individual::add_day(int &day, vector< pair<int, int>> &fam_day_perm, vector<bool> &grid_days, vector<int> &list_days, int &Real_size_list_days)
{
        if(!grid_days[day] )	
         {
	   grid_days[day]=true;
	   list_days[Real_size_list_days++] = day; 
	 }
	if(day>0 && !grid_days[day-1] )	
	    {
		grid_days[day-1] = true;
	        list_days[Real_size_list_days++] = day-1;
  	    }
}
bool Individual::my_next_combination(vector<int> &row_perm, const vector<int> &upper_opt, const vector<int> &fam_perm, vector<pair<int, int> > &fam_day_perm, int &Real_size, vector<bool> &grid_days, vector<int> &list_days, int &Real_size_list_days, vector<int> &x)
{
      int cont_nines = 0;
      Real_size  = 0;
      Real_size_list_days = 0;
      row_perm[0]++;     
      int size_w = row_perm.size()-1; 
	

      for(int col = 0; col < size_w; col++) //check all cols in range..
      {
	if(row_perm[col] > upper_opt[col])
	{
	 if(row_perm[col] >= upper_opt[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}

	if(row_perm[col] != NOT_CHECK)
	{
	 int day = domain[fam_perm[col]][row_perm[col]];
	 fam_day_perm[Real_size++] = make_pair(fam_perm[col], day);
         add_day(day, fam_day_perm, grid_days, list_days, Real_size_list_days);
         add_day(x[fam_perm[col]], fam_day_perm, grid_days, list_days, Real_size_list_days);
        }
      }
      if(row_perm[size_w] >= upper_opt[size_w]) cont_nines++;
      if(row_perm[size_w] != NOT_CHECK)
	{
	 int day = domain[fam_perm[size_w]][row_perm[size_w]];
	 fam_day_perm[Real_size++] = make_pair(fam_perm[size_w], day);
	 add_day(day, fam_day_perm, grid_days, list_days, Real_size_list_days);
         add_day(x[fam_perm[size_w]], fam_day_perm, grid_days, list_days, Real_size_list_days);
	}
   for(int i = 0; i < Real_size_list_days; i++) grid_days[list_days[i]]=false;

   if(cont_nines == size_w+1) return false;
   return true;
}
void Individual::try_all_permutations(struct Solution &S, const vector<int> &perm, double &best_local_score, vector<int> &best_local_perm_family, vector<int> &best_local_perm_days, int sub_domain_size)
{
   int k_subspace = best_local_perm_family.size(), Real_size=0;
   vector<int> row_perm(k_subspace, NOT_CHECK), upper_opt(k_subspace, sub_domain_size); //it can be optimized................
   for(int i = 0; i < k_subspace; i++) upper_opt[i] = 1+rand()%9;
   vector<pair<int, int>> fam_day_perm(k_subspace);
   vector<bool> grid_days(SW->N_DAYS, false);
   vector<int> list_days(SW->N_DAYS);
   int Real_size_list_days = 0;
   //while(my_next_combination(row_perm, upper_opt, perm, fam_day_perm, Real_size))//, grid_days, list_days, Real_size_list_days) )
   while(my_next_combination(row_perm, upper_opt, perm, fam_day_perm, Real_size, grid_days, list_days, Real_size_list_days, S.x) )
   {
     //check first feasibility and then score..
     double current_score;
     if(S.feasible)
     {
       //current_score = SW->incremental_evaluation(S, row_perm, perm);
       //current_score = SW->incremental_evaluation(S, fam_day_perm, Real_size);
       current_score = SW->incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
     }
     else
     {
       current_score = SW->incremental_evaluation_unfeasible(S, row_perm, perm);
//       if(current_score <= 0) //if this movement changes to feasible, then check the feasibility, therefore tthe unfeasibility score should be major than the feasible score..
//       current_score = SW->incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
//            current_score = SW->incremental_evaluation(S, fam_day_perm, Real_size);
//       //   current_score = -SW->incremental_evaluation(S, row_perm, perm);
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
