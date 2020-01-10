#include <signal.h>
#include "SantaWorkshop.h"
#include "Individual.h"
#include "utils.h"
using namespace std;


long long best = 1e16;
SantaWorkshop::SantaWorkshop(string file){
  load(file);
 // init_table_permutations(6);  
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
	if(opc==NOT_OPTION) 
          preference_costs[i][j] = 500.0 + 36.0*n + 398.0*n;
	else
          preference_costs[i][j] = c1[opc] + c2[opc]*n + c3[opc]*n;
      }
   }
  accounting_costs.resize(MAX_OCCUPANCY-MIN_OCCUPANCY+1, vector<double> (MAX_OCCUPANCY-MIN_OCCUPANCY+1, 1e6));

  for(int i = 0; i < MAX_OCCUPANCY-MIN_OCCUPANCY+1; i++)
  {
    for(int j = 0; j < MAX_OCCUPANCY-MIN_OCCUPANCY+1; j++)
     accounting_costs[i][j] =  (i/400.0)*(pow(i+125, 0.5 + (abs(i-j)/50.0)));
  }
}
double SantaWorkshop::Accounting_Penalty_Computation(vector<int> &daily_occupancy)
{
  double accounting_penalty = 0.0;
   for(int i = 0; i < 99; i++)
   {
 	int i2 = min(i+1, 99);
	if(daily_occupancy[i] > MAX_OCCUPANCY)
		accounting_penalty += 1e10*(daily_occupancy[i]-MAX_OCCUPANCY);
	else if(daily_occupancy[i] < MIN_OCCUPANCY)
		accounting_penalty += 1e10*(MIN_OCCUPANCY-daily_occupancy[i]);
	else if(daily_occupancy[i2] > MAX_OCCUPANCY)
		accounting_penalty += 1e10*(daily_occupancy[i2]-MAX_OCCUPANCY);
	else if(daily_occupancy[i2] < MIN_OCCUPANCY)
		accounting_penalty += 1e10*(MIN_OCCUPANCY-daily_occupancy[i2]);
	else
         accounting_penalty +=  accounting_costs[daily_occupancy[i]-125][daily_occupancy[i2]-125];
   }
  return accounting_penalty;
}
double SantaWorkshop::incremental_evaluation(vector<int> &x, const vector<int> &row_perm, const vector<int> &perm, double preference_penalty, vector<int> daily_occupancy)
{
     double accounting_penalty = 0.0;

    //preference penalty...
    for(int i = 0 ; i < row_perm.size(); i++)
    {
 	  if( row_perm[i] == NOT_CHECK) continue;	

	  int id_fam = perm[i];
	  int day_out = x[id_fam], day_in = domain[id_fam][row_perm[i]];
	  daily_occupancy[day_out] -= familiy_size[id_fam];
	  daily_occupancy[day_in] += familiy_size[id_fam];
	if(daily_occupancy[day_in] > MAX_OCCUPANCY)
		return 1e10*(daily_occupancy[day_in]-MAX_OCCUPANCY);
	else if(daily_occupancy[day_in] < MIN_OCCUPANCY)
		return 1e10*(MIN_OCCUPANCY-daily_occupancy[day_in]);
 	else if(daily_occupancy[day_out] < MIN_OCCUPANCY)
		return 1e10*(MIN_OCCUPANCY-daily_occupancy[day_out]);
	else if(daily_occupancy[day_out] > MAX_OCCUPANCY)
		return 1e10*(daily_occupancy[day_out]-MAX_OCCUPANCY);

	  preference_penalty +=  preference_costs[id_fam][day_in] - preference_costs[id_fam][day_out];
    }

   // accounting penalty
    accounting_penalty = Accounting_Penalty_Computation(daily_occupancy);

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
		return 1e10*(daily_occupancy[d]-MAX_OCCUPANCY);
 	  else if(daily_occupancy[d] < MIN_OCCUPANCY)
		return 1e10*(MIN_OCCUPANCY-daily_occupancy[d]);
   }

    accounting_penalty = Accounting_Penalty_Computation(daily_occupancy);
   return accounting_penalty + preference_penalty;
}

double SantaWorkshop::evaluate(vector<int> &x, vector<int> &daily_occupancy, double &preference_penalty)
{
    double accounting_penalty = 0.0;
    preference_penalty = 0.0;
    for(int i = 0; i < daily_occupancy.size(); i++) daily_occupancy[i] = 0;

    //preference penalty...
    for(int i = 0; i < x.size(); i++)
    {
	  preference_penalty += preference_costs[i][x[i]];
	  daily_occupancy[x[i]] +=familiy_size[i];
    }
   for(int d = 0 ; d < N_DAYS; d++) //it ould be a sum instead..
   {
	  if(daily_occupancy[d] > MAX_OCCUPANCY)
		return 1e10*(daily_occupancy[d]-MAX_OCCUPANCY);
 	  else if(daily_occupancy[d] < MIN_OCCUPANCY)
		return 1e10*(MIN_OCCUPANCY-daily_occupancy[d]);
   }
   // accounting penalty
    accounting_penalty = Accounting_Penalty_Computation(daily_occupancy);
   return accounting_penalty + preference_penalty;
}
void SantaWorkshop::init_table_permutations(int max_subspace_size)
{
   vector<int> row_perm(max_subspace_size, NOT_CHECK);
   vector<int> branch_1(max_subspace_size, 9); // limit the feasible space of each permutation if it is necessary
   int cont_nines = 0;
   while(cont_nines < max_subspace_size)
   {
      cont_nines = 0 ;
      vector<pair<int, int>> sparse_row;
      for(int i = 0; i < row_perm.size(); i++)
      if(row_perm[i] != NOT_CHECK) sparse_row.push_back(make_pair(i, row_perm[i]));
      table_permutations.push_back(sparse_row); 

      row_perm[0]++;     
      for(int col = 0; col < row_perm.size()-1; col++) //check all cols in range..
      {
	if(row_perm[col] > branch_1[col])
	{
	 if(row_perm[col] >= branch_1[col]) cont_nines++;
	  row_perm[col] = NOT_CHECK;
	  row_perm[col+1]++;
	}
      }
      if(row_perm[max_subspace_size-1] > branch_1[max_subspace_size-1]) cont_nines++;
   }  
}
