/*This try to create the ranking of the fitness*/

#include <algorithm>
using namespace std;
#define inf 50000;

bool fitcompare(individual fit1, individual fit2) { return fit1.total_viol < fit2.total_viol; }

void sorting(population *pop_ptr, int popsize, int m)
{
	int realsize = popsize*m;

	//for(int i=0;i<m*popsize;i++)
	//{
	//	
	//	pop_ptr->ind_ptr= &(pop_ptr->ind[i]);
	//	double m = 2;
	//}
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]); 
	sort(pop_ptr->ind_ptr, (pop_ptr->ind_ptr)+realsize, fitcompare);

	  	/* check the fitness part*/
	//for(int i=0;i<m*popsize;i++)
	//{
	//	
	//	pop_ptr->ind_ptr= &(pop_ptr->ind[i]);
 //		double m = 2;
	//}
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
}