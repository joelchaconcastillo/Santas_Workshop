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
     localSearch1(current_var, f_current);
     localSearch2(current_var, f_current);
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
     print(best_var);
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
  vector<int> current_var = original_var;
  double f_current = f_original;
  int max_ite = 100000, cont = 0;
  while(cont < max_ite )
  {
    //perturbe(current_var, 1);
    for(int x1 = 0; x1 < current_var.size(); x1++)
    {
      for(int x2 = 0; x2 < current_var.size(); x2++)
      { 
       int r1 = x1;//rand()%current_var.size();
       int r2 = x2;//rand()%current_var.size();
	if(current_var[x1] == current_var[x2]) continue;
      // while(current_var[r1] == current_var[r2])
      // r2 = rand()%current_var.size();
       iter_swap(current_var.begin()+r1, current_var.begin()+r2);


       f_current = calculateFitness(current_var);
       if(f_current < f_original)
       {
        f_original = f_current;
        original_var = current_var; 
        cout <<"-ls 3 --"  <<f_original <<endl;
        cont = 0;
       }
       else
       { 
          current_var = original_var;
          cont++;
       }
      }
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
