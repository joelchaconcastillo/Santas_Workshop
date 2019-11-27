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

	Santa_Workshop SW(file);
	//MA ma(N, pc, pm, finalTime, TTP);
	//ma.run();
	//
	///Temporal para probar la busqueda local...
	  Individual indiv(TTP);
	  pair<long long, int > p = indiv.calculateFitness(indiv.x_var);
  	  indiv.iterated_local_search();
	  p = indiv.calculateFitness(indiv.x_var);
	  cout <<  p.first<< " " <<  p.second<<endl;
	  SW.save_xml(TTP.x_var_room, TTP.x_var_time, TTP.x_var_student);
       

	  return 0;
}
