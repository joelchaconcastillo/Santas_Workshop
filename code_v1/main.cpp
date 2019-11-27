#include "MA.h"

int main(int argc, char **argv){
	int N = 1;
	double pc = 0.9;
	double pm = 0.01;
	double finalTime = 25 * 60;
	unsigned long l = 1572386715;
      //l = time(NULL);
	cout << l <<endl;
	srand(l);

	string file = "Instances/public/family_data.csv";

	SantaWorkshop SW(file);
	//MA ma(N, pc, pm, finalTime, TTP);
	//ma.run();
	//
	  Individual indiv(SW);
	  long long p = indiv.calculateFitness(indiv.x_var);
  	  cout << p << endl;
//  	  indiv.iterated_local_search();
//	  p = indiv.calculateFitness(indiv.x_var);
       

	  return 0;
}
