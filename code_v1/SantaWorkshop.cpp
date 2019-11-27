#include <signal.h>
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;

//////////////////////////Problem information///////////////////////////////////////
//TimeTablingProblem* TimeTablingProblem::TTP;

long long best = 1e16;
SantaWorkshop::SantaWorkshop(string file){
  load(file);
  
}
void SantaWorkshop::load_example(string file, vector<int> &x_var)
{
   FILE *fp = fopen( file.c_str(), "r");
   domain.resize( N_FAM, vector<int> (N_OPTIONS, 0));
   familiy_size.resize(N_FAM);
   char tmp[200];
   fscanf(fp, "%s\n", tmp);
   for(int i = 0; i < N_FAM; i++)
   {
     int id;
     fscanf(fp, "%d,", &id);
    fscanf(fp, "%d\n", &(x_var[id]));
   }
}
void SantaWorkshop::load(string file)
{
   FILE *fp = fopen( file.c_str(), "r");
   domain.resize( N_FAM, vector<int> (N_OPTIONS, 0));
   familiy_size.resize(N_FAM);
   char tmp[200];
   fscanf(fp, "%s\n", tmp);
   for(int i = 0; i < N_FAM; i++)
   {
     int id;
     fscanf(fp, "%d,", &id);
     for(int j =0; j < N_OPTIONS;j++)
        fscanf(fp, "%d,", &(domain[id][j]));
    fscanf(fp, "%d\n", &(familiy_size[id]));
   }
}
double SantaWorkshop::evaluate(vector<int> &x)
{
    double penalty = 0;
    vector<int> daily_occupancy(N_DAYS,0);
    vector<bool> checked(N_DAYS, false); 
    //preference cost...
    for(int i = 0; i < x.size(); i++)
    {
          int n = familiy_size[i];
	  int d = x[i];
	  if( d == 1) penalty += 50;
	  else if( d == 2) penalty += 50 + 9 *n;
	  else if( d == 3) penalty += 100 + 9 *n;
	  else if( d == 4) penalty += 200 + 9 *n;
	  else if( d == 5) penalty += 200 + 18 *n;
	  else if( d == 6) penalty += 300 + 18 *n;
	  else if( d == 7) penalty += 300 + 36 *n;
	  else if( d == 8) penalty += 400 + 36 *n;
	  else if( d == 9) penalty += 500 + 36 *n + 199*n;
	  else  penalty += 500 + 36 *n + 398*n;
	  daily_occupancy[d]  +=n;

	  //using soft constraints instead of hard constraints..
	  if( !checked[d] && (daily_occupancy[d] > MAX_OCCUPANCY || daily_occupancy[d] < MIN_OCCUPANCY))
	  {
	    penalty += 100000000;
	    checked[d] =true;
	  }
    }
   // accounting penalty
   //first day..
   double accounting_cost = (((daily_occupancy[x[0] - 125.0]))/400.0)*(pow(daily_occupancy[x[0]], 0.5));
   accounting_cost = max(0.0, accounting_cost);
   for(int i = 1; i < 100; i++)
   {
      double diff =  abs(daily_occupancy[i] - daily_occupancy[i+1]);
      accounting_cost += max(0.0, ((daily_occupancy[x[i] - 125.0])/400.0)*(pow(daily_occupancy[x[i]], 0.5 + (diff/50.0) )));
   }
   penalty += accounting_cost; 
   return penalty;
}
