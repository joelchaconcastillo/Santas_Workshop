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
    int v = rand()%10;
//    if(v < 10)
//       x[idx] = domain[idx][v];
//    else
       v = 1+(rand()%100);
    if( rand()%2 )
       x[idx] = v;
    else
    {
	iter_swap(x.begin()+(rand()%x.size()), x.begin()+(rand()%x.size()) );
    }
 }
}
void Individual::iterated_local_search()
{
  vector<int> current_var = x_var;
  vector<int> best_var = x_var;
  double f_best = calculateFitness(x_var);
  double f_current = calculateFitness(x_var);
  int np = 1; 
  SW->max_occupancy = SW->MAX_OCCUPANCY; 
  SW->min_occupancy = SW->MIN_OCCUPANCY; 
  while(true)
  {
cout << "a1: "<< calculateFitness(current_var) <<endl;
     double in = f_current;
     localSearch1(current_var, f_current);
//     localSearch2(current_var, f_current);
cout << "a2: "<< calculateFitness(current_var) <<endl;
     localSearch3(current_var, f_current);
     if(in > f_current) continue;
cout << "a3: "<< calculateFitness(current_var) <<endl;
	//exit(0);
     if(f_current < f_best)
     {
	cout << f_current <<endl;
        //localSearch3(current_var, f_current);
	cout << f_current <<endl;
        f_best = f_current;
        best_var = current_var;

     print(best_var);
	np=1;
     }
     else
     {
       current_var = best_var;
       f_current = f_best;
     }
//     SW->max_occupancy = SW->MAX_OCCUPANCY + 10; 
//     SW->min_occupancy = SW->MIN_OCCUPANCY - 10; 
//     localSearch2(current_var, f_current);
//  SW->max_occupancy = SW->MAX_OCCUPANCY; 
//  SW->min_occupancy = SW->MIN_OCCUPANCY; 

     perturbe(current_var, 2);
     f_current = calculateFitness(current_var);
     cout << f_best <<endl;
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
    //random_shuffle(idx.begin(), idx.end());
   improved = false;
     
  for(int l = 0; l < original_var.size(); l++)
  {
	int i = idx[l];
     for(int j = 0 ;  j < domain[i].size(); j++)
//     for(int j = 1 ;  j <= 100; j++)
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
	}
     }
  }
  }

	  //cout << "ls1--" <<f_original <<endl;
}
void Individual::localSearch2(vector<int> & original_var, double &f_original)
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
    while(cont < 200)
    {
    //perturbe(current_var, 1);
	int x1 = rand()%current_var.size();
	int x2 = rand()%current_var.size();
       if(x1==x2 || current_var[x1] == current_var[x2]) continue; //same fam or day..
        double value = SW->incremental_evaluation(current_var, x1, x2, daily_occupancy);
       if(value-0.001  > 0.0)
       {
        int day_a = current_var[x1], day_b = current_var[x2];
	daily_occupancy[day_a] = daily_occupancy[day_a] - familiy_size[x1] + familiy_size[x2];
        daily_occupancy[day_b] = daily_occupancy[day_b] - familiy_size[x2] + familiy_size[x1];

	iter_swap(current_var.begin()+x1, current_var.begin()+x2);
	improved = true;
        f_original = calculateFitness(current_var);
        original_var = current_var; 
	cont = 0;
       }
	else cont++;
    }

  }
	//cout << calculateFitness(original_var)<<endl;

}

void Individual::localSearch3(vector<int> & original_var, double &f_original)
{
  int N_DAYS = 100;
  vector<int> familiy_size = SW->familiy_size;
  vector<int> current_var = original_var;
  vector<int> daily_occupancy(N_DAYS+1,0);
  vector<int> perm;
  for(int i = 0 ; i < current_var.size(); i++)
  {
    daily_occupancy[current_var[i]]+= SW->familiy_size[i];
   perm.push_back(i);
  }

  double f_current = f_original;
  int max_ite = 100000, cont = 0;
  bool improved = true;
  while(improved)
  {
	improved =false;
    random_shuffle(perm.begin(), perm.end());
    //perturbe(current_var, 1);
    for(int i = 0; i < current_var.size(); i++)
    {
	int x1 = perm[i];
      for(int j = 0; j < current_var.size(); j++)
      { 
	int x2 = perm[j];
	if(x1==x2 || current_var[x1] == current_var[x2]) continue; //same fam or day..
        vector<int> tmp = original_var;
        double f_tmp = f_original;
        
	 iter_swap(tmp.begin()+x1, tmp.begin()+x2);
        f_tmp = calculateFitness(tmp);

        if( f_tmp < f_original)
        {
		f_original = f_tmp;
		original_var = tmp;
        }


      //  double value = SW->incremental_evaluation(current_var, x1, x2, daily_occupancy);
      // if(value-0.001  > 0.0)
      // {
      //  int day_a = current_var[x1], day_b = current_var[x2];
      //  daily_occupancy[day_a] = daily_occupancy[day_a] - familiy_size[x1] + familiy_size[x2];
      //  daily_occupancy[day_b] = daily_occupancy[day_b] - familiy_size[x2] + familiy_size[x1];

      //  iter_swap(current_var.begin()+x1, current_var.begin()+x2);
      //  improved = true;
      //  f_original = calculateFitness(current_var);
      //  original_var = current_var; 
      // }

      }
    }
  }
	//cout << calculateFitness(original_var)<<endl;
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
