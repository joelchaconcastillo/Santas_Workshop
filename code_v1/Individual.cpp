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
     localSearch(current_var, f_current);
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
void Individual::localSearch(vector<int> & original_var, double &f_original)
{
  vector<int> current_var = original_var;
  double f_current = f_original;
  int N_DAYS =100;
  vector<vector<int>> day_var(N_DAYS+1);
  for(int i = 0; i < original_var.size(); i++)
	day_var[original_var[i]].push_back(i);

  int max_ite = 100000, cont = 0;
  while(cont < max_ite )
  {
    //perturbe(current_var, 1);
    int r1 = (rand()%N_DAYS)+1;
    while( day_var[r1].empty()) r1 = (rand()%N_DAYS)+1;
//    int r11 = rand()%day_var[r1].size();
    for(int i =  day_var[r1].size()-1; i>=0; i--)
    {
	int id_fam = day_var[r1][i];
       for(int j = 1; j <=100; j++) 
       {
         // day_var[r2].push_back(day_var[r1][r11]);    
         // iter_swap(day[r1].begin()+r11, dat[r1].end()-1);
         // day[r1].pop_back();
          current_var[id_fam] = j;
           
          f_current = calculateFitness(current_var);
          if(f_current < f_original)
          {
           f_original = f_current;
           original_var = current_var; 
           cout <<"---"  <<f_original <<endl;
	   cont = 0;
          }
          else
          { 
             current_var = original_var;
             cont++;
          }
       }
	 day_var[original_var[id_fam]].push_back(id_fam);
          iter_swap(day_var[r1].begin()+i, day_var[r1].end()-1);
	  day_var[r1].pop_back();

    }
  }
}
//void Individual::localSearch(vector<int> & original_var, double &f_original)
//{
//  vector<int> current_var = original_var;
//  double f_current = f_original;
//  int max_ite = 100000, cont = 0;
//  while(cont < max_ite )
//  {
//    //perturbe(current_var, 1);
//    int r1 = rand()%current_var.size();
//    int r2 = rand()%current_var.size();
//    iter_swap(current_var.begin()+r1, current_var.begin()+r2);
//    f_current = calculateFitness(current_var);
//    if(f_current < f_original)
//    {
//     f_original = f_current;
//     original_var = current_var; 
//     cout <<"---"  <<f_original <<endl;
//     cont = 0;
//    }
//    else
//    { 
//       current_var = original_var;
//       cont++;
//    }
//  }
//}
//
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
