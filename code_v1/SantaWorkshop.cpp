#include <signal.h>
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;


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
     int id_fam, day;
     fscanf(fp, "%d,", &id_fam);
     fscanf(fp, "%d\n", &day);
     x_var[id_fam] = day;//inv_domain[id][day];
   }
}
void SantaWorkshop::load(string file)
{
   FILE *fp = fopen( file.c_str(), "r");
   domain.resize( N_FAM, vector<int> (N_OPTIONS, 0));
   inv_domain.resize( N_FAM, vector<int> (N_DAYS+1, NOT_OPTION));

   familiy_size.resize(N_FAM);
   char tmp[200];
   fscanf(fp, "%s\n", tmp);
   for(int i = 0; i < N_FAM; i++)
   {
     int id;
     fscanf(fp, "%d,", &id);
     for(int j =0; j < N_OPTIONS; j++)
     {
        fscanf(fp, "%d,", &(domain[id][j]));
	inv_domain[id][domain[id][j]] = j; 
     }
    fscanf(fp, "%d\n", &(familiy_size[id]));
   }
   c1.resize(11);
   c1[0] = 0.0;
   c1[1] = 50.0;
   c1[2] = 50.0;
   c1[3] = 100.0;
   c1[4] = 200.0;
   c1[5] = 200.0;
   c1[6] = 300.0;
   c1[7] = 300.0;
   c1[8] = 400.0;
   c1[9] = 500.0;
   c1[10] = 500.0;

   c2.resize(11);
   c2[0] = 0.0;
   c2[1] = 0.0;
   c2[2] = 9.0;
   c2[3] = 9.0;
   c2[4] = 9.0;
   c2[5] = 18.0;
   c2[6] = 18.0;
   c2[7] = 36.0;
   c2[8] = 36.0;
   c2[9] = 36.0;
   c2[10] = 36.0;

   c3.resize(11);
   c3[0] = 0.0;
   c3[1] = 0.0;
   c3[2] = 0.0;
   c3[3] = 0.0;
   c3[4] = 0.0;
   c3[5] = 0.0;
   c3[6] = 0.0;
   c3[7] = 0.0;
   c3[8] = 0.0;
   c3[9] = 199.0;
   c3[10] = 398.0;
   preference_costs.resize(N_FAM, vector<double> (N_DAYS+1));
   for(int i = 0; i < N_FAM; i++)
   {
      int n = familiy_size[i];
      for(int  j = 1; j <=N_DAYS; j++)
      {	
	int opc = inv_domain[i][j];
	if(opc!=NOT_OPTION) 
          preference_costs[i][j] = c1[opc] + c2[opc]*n + c3[opc]*n;
	else
          preference_costs[i][j] = 500.0 + 36.0*n + 398.0*n;
      }
   }
  accounty_penalty.resize(MAX_OCCUPANCY+1, vector<double> (MAX_OCCUPANCY+1, 1e6));
  for(int i = MIN_OCCUPANCY-1; i <= MAX_OCCUPANCY; i++)
  {
    for(int diff =0; diff <= MAX_OCCUPANCY-MIN_OCCUPANCY+1; diff++)
     accounty_penalty[i][diff] = max(0.0, ((i - 125.0)/400.0)*(pow(i, 0.5 + (diff/50.0) )));
  }
}
double SantaWorkshop::partial_accounting_cost(int id_day, vector<int> &daily_occupancy)
{
  int id_day_next = min(id_day+1, N_DAYS);
  int diff = fabs( daily_occupancy[id_day] - daily_occupancy[id_day_next] );

  double cost = max(0.0, ((daily_occupancy[id_day] - 125.0)/400.0)*(pow(daily_occupancy[id_day], 0.5 + (diff/50.0) )));
   //if( daily_occupancy[id_day] <MIN_OCCUPANCY ||  daily_occupancy[id_day] >MAX_OCCUPANCY) cost+=1e5;
   if( daily_occupancy[id_day] < min_occupancy ||  daily_occupancy[id_day] >max_occupancy) cost+=1e5;
//  double cost = accounty_penalty[daily_occupancy[id_day] ][diff];

  if(id_day <= 1) return cost;
  id_day--;
  id_day_next =id_day+1;
  diff = fabs( daily_occupancy[id_day] - daily_occupancy[id_day_next] );
  cost += max(0.0, ((daily_occupancy[id_day] - 125.0)/400.0)*(pow(daily_occupancy[id_day], 0.5 + (diff/50.0) )));
//  cost += accounty_penalty[ daily_occupancy[id_day] ][diff];
  return cost;
}
double SantaWorkshop::incremental_evaluation(vector<int> &x, int fam_i, int fam_j, vector<int> &daily_occupancy)
{
   int day_a = x[fam_i];
   int day_b = x[fam_j];
   double total_preference_cost = preference_costs[fam_i][day_a] + preference_costs[fam_j][day_b]  - preference_costs[fam_i][day_b] - preference_costs[fam_j][day_a];

   int new_ocup_a = daily_occupancy[day_a] - familiy_size[fam_i] + familiy_size[fam_j];
   int new_ocup_b = daily_occupancy[day_b] + familiy_size[fam_i] - familiy_size[fam_j];



   double cost_Ni1 =  partial_accounting_cost(day_a, daily_occupancy);
   double cost_Nj1 =  partial_accounting_cost(day_b, daily_occupancy);
   //temporal update of daily..
   daily_occupancy[day_a] = new_ocup_a;//+ (- familiy_size[fam_i] + familiy_size[fam_j]);
   daily_occupancy[day_b] = new_ocup_b;//+= (- familiy_size[fam_j] + familiy_size[fam_i]);

   double cost_Ni2 =  partial_accounting_cost(day_a, daily_occupancy);
   double cost_Nj2 =  partial_accounting_cost(day_b, daily_occupancy);
// cout << cost_Ni1 << " "<<cost_Nj1 << " "<<cost_Ni2 << " "<<cost_Nj2 <<endl; 

  ///get back original values..
   daily_occupancy[day_a] += (familiy_size[fam_i] - familiy_size[fam_j]);
   daily_occupancy[day_b] += (familiy_size[fam_j] - familiy_size[fam_i]);

   double total_accounting_cost = cost_Ni1 + cost_Nj1 - cost_Ni2 - cost_Nj2;

   return total_preference_cost+total_accounting_cost;
}
double SantaWorkshop::evaluate(vector<int> &x)
{
    double penalty = 0.0;
    vector<int> daily_occupancy(N_DAYS+1,0);
    //preference cost...
    for(int i = 0; i < x.size(); i++)
    {
          int n = familiy_size[i];
	  int d = x[i];
	  penalty += preference_costs[i][d];
//	  bool updated = false;
//	  for(int j = 0; j < 10; j++)
//	    if( d == domain[i][j])
//	    {
//		 updated = true;
//		 penalty += c1[j] + c2[j]*n + c3[j]*n;
//	    }
//	  if(!updated)penalty += 500.0 + 36.0*n + 398.0*n;

//	  if( d == domain[i][0]) penalty += 0.0;
//	  else if( d == domain[i][1] ) penalty += 50.0;
//	  else if( d == domain[i][2] ) penalty += 50.0 + 9.0 *n;
//	  else if( d == domain[i][3] ) penalty += 100.0 + 9.0 *n;
//	  else if( d == domain[i][4] ) penalty += 200.0 + 9.0 *n;
//	  else if( d == domain[i][5] ) penalty += 200.0 + 18.0 *n;
//	  else if( d == domain[i][6]) penalty += 300.0 + 18.0 *n;
//	  else if( d == domain[i][7]) penalty += 300.0 + 36.0 *n;
//	  else if( d == domain[i][8]) penalty += 400.0 + 36.0 *n;
//	  else if( d == domain[i][9]) penalty += 500.0 + 36.0 *n + 199.0*n;
//	  else penalty += 500.0 + 36.0*n + 398.0*n;
	  daily_occupancy[d] +=n;
    }
   for(int d = 1 ; d <= N_DAYS; d++)
	  if( (daily_occupancy[d] > MAX_OCCUPANCY || daily_occupancy[d] < MIN_OCCUPANCY))
	    penalty += 100000.0;
   // accounting penalty
   //first day..
   double accounting_cost = ((daily_occupancy[100] - 125.0)/400.0)*(pow(daily_occupancy[100], 0.5));
   accounting_cost = max(0.0, accounting_cost);
   for(int i = 99; i >= 1; i--)
   {
      double diff =  fabs(daily_occupancy[i] - daily_occupancy[i+1]);
      accounting_cost += max(0.0, ((daily_occupancy[i] - 125.0)/400.0)*(pow(daily_occupancy[i], 0.5 + (diff/50.0) )));
   }
//   double accounting_cost = 0.0;//((daily_occupancy[100] - 125.0)/400.0)*(pow(daily_occupancy[100], 0.5));
//   accounting_cost = max(0.0, accounting_cost);
//   for(int i = 1; i <= N_DAYS; i++)
//   {
//      int diff = 0;
//      if(i< N_DAYS)  diff = fabs(daily_occupancy[i] - daily_occupancy[i+1]);
//      if(diff < accounty_penalty[0].size())
//      accounting_cost += accounty_penalty[daily_occupancy[i]][diff];// max(0.0, ((daily_occupancy[i] - 125.0)/400.0)*(pow(daily_occupancy[i], 0.5 + (diff/50.0) )));
//   }

   penalty += accounting_cost; 
   return penalty;
}
