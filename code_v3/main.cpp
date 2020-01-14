#include "MA.h"

int main(int argc, char **argv){
	int N = 10;
	double pc = 0.9;
	double pm = 0.01;
	double finalTime = 60 * 60;
	unsigned long l = 1578701648;
 //       l=1;
      l = time(NULL);
	cout << l <<endl;
	srand(l);

	string file = "Instances/public/family_data.csv";

	SantaWorkshop SW(file);
	MA ma(N, pc, pm, finalTime, SW);
	ma.run();
   return 0;
	//
	  Individual indiv(SW);
	  //SW.load_example("Instances/public/submission_672254.0276683343.csv", indiv.x_var);
	  SW.load_example("o", indiv.x_var);
          //indiv.iterated_local_search();
	  indiv.subspace_local_search();
//	  double p = indiv.calculateFitness(indiv.x_var);
//	  printf("%lf", p);
//  	  cout << p << endl;
//	  p = indiv.calculateFitness(indiv.x_var);
       

	  return 0;
}
