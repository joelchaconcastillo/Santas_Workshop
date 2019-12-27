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

void Individual::perturbe(vector<int> &x, int times)
{
 for(int i = 0; i < times; i++)
 {
    int idx = rand()%x.size();  
    int v = rand()%11;
    if(v < 10)
       x[idx] = domain[idx][v];
    else
       x[idx] = 1+(rand()%100);
 }
}
void Individual::iterated_local_search()
{
  vector<int> current_var = x_var;
  vector<int> best_var = x_var;
  double f_best = calculateFitness(x_var);
  double f_current = calculateFitness(x_var);
  int np = 1; 
  while(true)
  {
//     localSearch1(current_var, f_current);
//     localSearch2(current_var, f_current);
     localSearch3(current_var, f_current);
     if(f_current < f_best)
     {
        f_best = f_current;
        best_var = current_var;
	np=1;
     }
     else
     {
       current_var = best_var;
       f_current = f_best;
	np++;
        np = max(np, 2);
     }
     perturbe(current_var, np);
     f_current = calculateFitness(current_var);
     cout << f_best <<endl;
  //   print(best_var);
  }


}
void Individual::localSearch1(vector<int> & original_var, double &f_original)
{
  vector<int> current_var = original_var;
  double f_current = f_original;
   bool improved = true;
  vector<int> idx(original_var.size());
  for(int i = 0 ; i < idx.size(); i++)
  idx[i]=i;
  while(improved)
  { 
    random_shuffle(idx.begin(), idx.end());
   improved = false;
     
  for(int l = 0; l < original_var.size(); l++)
  {
	int i = idx[l];
     for(int j = 0 ;  j < domain[i].size(); j++)
     {
   	vector<int> current_var = original_var;
  	double f_current = f_original;
	current_var[i] = domain[i][j];
	f_current = calculateFitness(current_var);
	if(f_current< f_original)
	{
	  improved = true;
	  original_var = current_var;
	  f_original = f_current;
	  cout << "ls1--" <<f_original <<endl;
	}
     }
  }
  }
}
void Individual::localSearch2(vector<int> & original_var, double &f_original)
{
  vector<int> current_var = original_var;
  double f_current = f_original;
  int max_ite = 100000, cont = 0;
  while(cont < max_ite )
  {
    perturbe(current_var, 1);
    f_current = calculateFitness(current_var);
    if(f_current < f_original)
    {
     f_original = f_current;
     original_var = current_var; 
     cout <<"-ls 2--"  <<f_original <<endl;
     cont = 0;
    }
    else
    { 
       current_var = original_var;
       cont++;
    }
  }

}

void Individual::localSearch3(vector<int> & original_var, double &f_original)
{
  int N_DAYS = 100;
  vector<int> familiy_size = SW->familiy_size;
  vector<int> current_var = original_var;
  vector<int> daily_occupancy(N_DAYS+1,0);
  for(int i = 0 ; i < current_var.size(); i++)
    daily_occupancy[current_var[i]]+= SW->familiy_size[i];

  double f_current = f_original;
  int max_ite = 100000, cont = 0;
  bool improved = true;
  while(improved)
  {
	improved =false;
    //perturbe(current_var, 1);
    for(int x1 = 0; x1 < current_var.size(); x1++)
    {
      for(int x2 = 0; x2 < current_var.size(); x2++)
      { 
	if(x1==x2) continue;
	if( SW->familiy_size[x1] != SW->familiy_size[x2])continue;
      
        double value = SW->incremental_evaluation(current_var, x1, x2, daily_occupancy);
       if(value  > 0)
       {
        int day_a = current_var[x1], day_b = current_var[x2];
	daily_occupancy[day_a] = daily_occupancy[day_a] - familiy_size[x1] + familiy_size[x2];
        daily_occupancy[day_b] = daily_occupancy[day_b] - familiy_size[x2] + familiy_size[x1];

	iter_swap(current_var.begin()+x1, current_var.begin()+x2);
	cout << calculateFitness(current_var)<<endl;
	improved = true;
        f_original = calculateFitness(current_var);
        original_var = current_var; 
       }

      }
    }
	cout << "----------"<<endl;
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
    cout <<i <<","<<sol[i]<<endl; 
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
