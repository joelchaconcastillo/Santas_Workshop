#include <omp.h>
#include <signal.h>
#include <chrono> 
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;
using namespace std::chrono; 

double Individual::calculateFitness(vector<int> &x_ind){
  return SW.evaluate(x_ind);
}

void printBest(){
	
}

void Individual::subspace_local_search()
{

  struct Solution S;
  S.score = fitness;
  S.x = x_var;
  SW.evaluate(S);


  int N_training = 1000;
  //fill a pool of random families...
  vector< vector<int> > permutation_families;
  vector<int> fam_perm;
  for(int i = 0; i < x_var.size(); i++) fam_perm.push_back(i);

/// //Training configuration...
/// //random ampling permutaitons of families...
/// for(int i = 0;  i < N_training; i++)
/// {
///   random_shuffle(fam_perm.begin(), fam_perm.end());
///   permutation_families.push_back(vector<int>(fam_perm.begin(), fam_perm.begin()+10));
/// }
///
/// vector<int> opts_subspace_size, opts_sub_domain_size;
pair<int, pair<int, int> > best_parameterization; //score, sub space size and sub space domain
/// opts_subspace_size.push_back(1);
/// opts_subspace_size.push_back(2);
/// opts_subspace_size.push_back(3);
/// opts_subspace_size.push_back(4);
///
/// opts_sub_domain_size.push_back(1);
/// opts_sub_domain_size.push_back(5);
/// opts_sub_domain_size.push_back(9);
///best_parameterization.second.first = opts_subspace_size[0]; 
///best_parameterization.second.second = opts_sub_domain_size[0]; 
///best_parameterization.first = S.score;
///
///
/// for(int a = 0; a < opts_subspace_size.size(); a++)
/// {
///  for(int b = 0; b < opts_sub_domain_size.size(); b++)
///  {  
///    struct Solution Stmp = S;
///    double current_score = S.score;
///   for(int i = 0 ; i  < permutation_families.size(); i++)
///   {
///        try_all_permutations(S, permutation_families[i], current_score, best_local_perm_family, best_local_perm_days, opts_sub_domain_size[b], opts_subspace_size[a]); //it replaces the best solution..
///        if( current_score < S.score-1e-10) 
///	{
///	   for(int i = 0 ; i < best_parameterization.second.first; i++)
///	   {
///	     if(best_local_perm_days[i] == NOT_CHECK)continue;
///	     Stmp.x[best_local_perm_family[i]] = domain[best_local_perm_family[i]][best_local_perm_days[i]];
///	   }
///   	   SW.evaluate(Stmp);
///	}
///   }
///   if( Stmp.score < best_parameterization.first )
///   {
///	  best_parameterization.first = Stmp.score;
///	  best_parameterization.second.first = opts_subspace_size[a]; 
///	  best_parameterization.second.second = opts_sub_domain_size[b]; 
///   }
///  }
/// }
vector<int> best_local_perm_family(2), best_local_perm_days(2); //variables to find the local optimal..
best_parameterization.second.first = 2;
best_parameterization.second.second = 9;
 cout << "best config K:" << best_parameterization.second.first<<" opts:"<<best_parameterization.second.second << endl;

    best_local_perm_family.resize(best_parameterization.second.first);
    best_local_perm_days.resize(best_parameterization.second.first);
 //////testing data.....
     int maxite = 20, cont=0, ite=0;
     double best_local_score = S.score;
   //  vector<int> best_local_perm_family(best_parameterization.second.first), best_local_perm_days(best_parameterization.second.first); //variables to find the local optimal..
//	cout << SW.my_random_shuffle.size();
     while(cont < maxite)
     {
	for(int i = 0; i < 100; i++)
	{
         // random_shuffle(fam_perm.begin(), fam_perm.end());
          try_all_permutations(S, SW.my_random_shuffle[(ite++)%SW.my_random_shuffle.size()], best_local_score, best_local_perm_family, best_local_perm_days, best_parameterization.second.second, best_parameterization.second.first); //it replaces the best solution..
	}
        if( best_local_score < S.score-1e-10) 
	{
	   for(int i = 0 ; i < best_parameterization.second.first; i++)
	   {
	     if(best_local_perm_days[i] == NOT_CHECK)continue;
	     S.x[best_local_perm_family[i]] = domain[best_local_perm_family[i]][best_local_perm_days[i]];
	   }
   	   SW.evaluate(S);
	   cont = 0;
	}
	cont++;
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
void Individual::try_all_permutations(struct Solution &S, const vector<int> &perm, double &best_local_score, vector<int> &best_local_perm_family, vector<int> &best_local_perm_days, int sub_domain_size, int k_subspace)
{
   int Real_size = 0;
   vector<int> row_perm(k_subspace, NOT_CHECK), upper_opt(k_subspace, sub_domain_size); //it can be optimized................
   //for(int i = 0; i < k_subspace; i++) upper_opt[i] = 1+rand()%9;
   vector<pair<int, int>> fam_day_perm(k_subspace);
   vector<bool> grid_days(SW.N_DAYS, false);
   vector<int> list_days(SW.N_DAYS);
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
       current_score = SW.incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
     }
     else
     {
       current_score = SW.incremental_evaluation_unfeasible(S, row_perm, perm);
       if(current_score <= 0) //if this movement changes to feasible, then check the feasibility, therefore tthe unfeasibility score should be major than the feasible score..
       current_score = SW.incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
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
void Individual::try_all_permutations(struct Solution &S, const vector<int> &perm, double &best_local_score, int sub_domain_size, int k_subspace)
{
   int Real_size=0;
   vector<int> row_perm(k_subspace, NOT_CHECK), upper_opt(k_subspace, sub_domain_size); //it can be optimized................
   vector<pair<int, int>> fam_day_perm(k_subspace);
   vector<bool> grid_days(SW.N_DAYS, false);
   vector<int> list_days(SW.N_DAYS);
   int Real_size_list_days = 0;
   while(my_next_combination(row_perm, upper_opt, perm, fam_day_perm, Real_size, grid_days, list_days, Real_size_list_days, S.x) )
   {
     //check first feasibility and then score..
     double current_score;
     if(S.feasible)
       current_score = SW.incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
     else
     {
       current_score = SW.incremental_evaluation_unfeasible(S, row_perm, perm);
       if(current_score <= 0) //if this movement changes to feasible, then check the feasibility, therefore tthe unfeasibility score should be major than the feasible score..
       current_score = SW.incremental_evaluation(S, fam_day_perm, Real_size, list_days, Real_size_list_days);
     }
     if( best_local_score > current_score )
	best_local_score = current_score;
   }
}
void Individual::localSearch()
{
 subspace_local_search();
}
int Individual::getDistance(Individual &ind){
   int dist = 0;
   for(int i = 0; i  < ind.x_var.size(); i++)
      dist += (ind.x_var[i]!=x_var[i]);	
   return dist;
}
void Individual::Mutation(double pm){
   for(int i = 0; i < x_var.size(); i++)
     if(generateRandomDouble0_Max(1) < pm)
	x_var[i] = domain[i][rand()%domain[i].size()];
   
}
void Individual::Crossover(Individual &ind){
  //uniform crossover...
  for(int i = 0 ; i < ind.x_var.size(); i++)
     if(generateRandomDouble0_Max(1)<0.5)
	swap(ind.x_var[i], this->x_var[i]);	
}
void Individual::print(vector<int> &sol){
  cout << "family_id,assigned_day"<<endl;
  for(int i = 0; i < sol.size(); i++)
  {
    cout <<i <<","<<sol[i]+1<<endl; 
  }
}
void Individual::print(){
  cout << "family_id,assigned_day"<<endl;
  for(int i = 0; i < x_var.size(); i++)
  {
    cout <<i <<","<<x_var[i]+1<<endl; 
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
