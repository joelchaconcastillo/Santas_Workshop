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
     x_var[id_fam] = day-1;
   }
}
void SantaWorkshop::load(string file)
{
   FILE *fp = fopen( file.c_str(), "r");
   domain.resize( N_FAM, vector<int> (N_OPTIONS, 0));
   inv_domain.resize( N_FAM, vector<int> (N_DAYS, NOT_OPTION));

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
	domain[id][j]--;
	inv_domain[id][domain[id][j]] = j; 
     }
    fscanf(fp, "%d\n", &(familiy_size[id]));
   }
   double c1[] = {0.0, 50.0, 50.0, 100.0, 200.0, 200.0, 300.0, 300.0, 400.0, 500.0, 500.0};
   double c2[] = {0.0, 0.0, 9.0, 9.0, 9.0, 18.0, 18.0, 36.0, 36.0, 36.0, 36.0};
   double c3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 199.0, 398.0};

   preference_costs.resize(N_FAM, vector<double> (N_DAYS));
   for(int i = 0; i < N_FAM; i++)
   {
      int n = familiy_size[i];
      for(int  j = 0; j < N_DAYS; j++)
      {	
	int opc = inv_domain[i][j];
	if(opc!=NOT_OPTION) 
          preference_costs[i][j] = 500.0 + 36.0*n + 398.0*n;
	else
          preference_costs[i][j] = c1[opc] + c2[opc]*n + c3[opc]*n;
      }
   }

  accounting_costs.resize(MAX_OCCUPANCY-MIN_OCCUPANCY+1, vector<double> (MAX_OCCUPANCY-MIN_OCCUPANCY+1, 1e6));

  for(int i = 0; i <= MAX_OCCUPANCY-MIN_OCCUPANCY+1; i++)
  {
    for(int j=0; j <= MAX_OCCUPANCY-MIN_OCCUPANCY+1; j++)
     accounting_costs[i][j] =  (i/400.0)*(pow(i+125, 0.5 + (abs(i-j)/50.0)));
  }
}
double SantaWorkshop::incremental_evaluation(vector<int> &X, vector<int> &indices, vector<int> &daily_occupancy, vector<int> &change)
{
     double preference_penalty = 0.0, accounting_penalty = 0.0;
    //preference penalty...
    for(int i = 0; i < indices.size(); i++)
    {
	  int id_fam_i = indices[i], id_fam_j = change[i];
	  daily_occupancy[x[id_fam_i]] -= familiy_size[id_fam_i];
	  daily_occupancy[x[id_fam_j]] += familiy_size[id_fam_j];
	  preference_penalty+= preference_costs[i][x[i]];
    }

   for(int d = 0 ; d < N_DAYS; d++) //it ould be a sum instead..
   {
	  if(daily_occupancy[d] > MAX_OCCUPANCY)
		return 1e12*(daily_occupancy[d]-MAX_OCCUPANCY);
 	  else if(daily_occupancy[d] < MIN_OCCUPANCY)
		return 1e12*(MIN_OCCUPANCY-daily_occupancy[d]);
   }
   // accounting penalty
   for(int i = 0; i < 99; i++)
      accounting_penalty+=  accounting_costs[daily_occupancy[i]-125][daily_occupancy[i+1]-125];
    accounting_penalty +=  accounting_costs[daily_occupancy[99]-125][daily_occupancy[99]-125];

   return accounting_penalty + preference_penalty;  
}
double SantaWorkshop::evaluate(vector<int> &x)
{
    double preference_penalty = 0.0, accounting_penalty = 0.0;
    vector<int> daily_occupancy(N_DAYS,0);

    //preference penalty...
    for(int i = 0; i < x.size(); i++)
    {
	  preference_penalty+= preference_costs[i][x[i]];
	  daily_occupancy[x[i]] +=familiy_size[i];
    }
   for(int d = 0 ; d < N_DAYS; d++) //it ould be a sum instead..
   {
	  if(daily_occupancy[d] > MAX_OCCUPANCY)
		return 1e12*(daily_occupancy[d]-MAX_OCCUPANCY);
 	  else if(daily_occupancy[d] < MIN_OCCUPANCY)
		return 1e12*(MIN_OCCUPANCY-daily_occupancy[d]);
   }
   // accounting penalty
   for(int i = 0; i < 99; i++)
      accounting_penalty+=  accounting_costs[daily_occupancy[i]-125][daily_occupancy[i+1]-125];
    accounting_penalty +=  accounting_costs[daily_occupancy[99]-125][daily_occupancy[99]-125];

   return accounting_penalty + preference_penalty;
}
