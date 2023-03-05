// HS_one route.cpp : Defin es the entry point for the console application.
// to make consistent with the MILP, B_b,i,j(k) can also be considered as decision variables, like x_b(k) and Q_b,i(k)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "stdafx.h"

#include<sys/types.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "string.h"


#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

#define PI 3.141592
#define square(x) ((x)*(x))
#define times 15
#define maxpop 110  /*Max population */ // >popsize 
const int Gen = 500; // running times
const int MaxBusStopNum = 20;// max row number 30            //////
const int MaxBusNum = 7;// max column number 20              //////
const int MaxTimeInt = 12; // max prediction horizon 30      //////
const int popsize = 50;  /*Population Size*/

const int MaxBusSpace = 100;

#define BoardTime 2 // 2s per person
#define AlightTime 1.5
#define OpenTime 2
#define DwellTime 180 // 3 min


using namespace std;
ofstream outfile;
FILE* fp;
errno_t err;

int Best_fitness;
int Best_violation;
// incoming passenger flow for each bus stop
int FlowIn[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];
int BOARD_VOLUME[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];
int DISPATCH[MaxTimeInt][MaxBusNum];

double VolumeProportion1[MaxTimeInt][MaxBusStopNum]; //= {1, 5, 3, 15, 17, 25, 2, 3, 9, 8, 32, 1, 2, 40, 3};
//int VolumeProportion2[TripNum][StopNum];// = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
double DestinationProportion1[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];
int Passenger_come;

// volume in buses

/*Chromosome Size*/
static int BusNum = 3;
static int BusStopNum = 10;
static int Num_Int = 10;
static int returnHorizon = 6;

typedef struct       /*individual properties*/
{
	int  genes_dispatch[MaxTimeInt][MaxBusNum]; /*bianry chromosome*/
	int BoardVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];

	int fitness;
	int space;
	int Acdelay; // Actual Delay
	int Pedelay; // Perceived Delay

	int Constraint_1_viol;
	int Constraint_2_viol;
	int Constraint_3_viol;
	int Constraint_4_viol;
	int Constraint_5_viol;

	int total_viol;
	//int boardvolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum]; 
	//int busvolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum]; 

	int DataLoadTime[MaxTimeInt][MaxBusNum][MaxBusStopNum]; // loading time for bus b of trip k at stop i
	//int DataTrueDwellTime[MaxTimeInt][MaxBusStopNum];       // maximum loading time for buses of trip k <= DwellTime (180s in fitness.h): fixed stopping time of buses of trip k at stop i 
}individual;        /*Structure defining individual*/


typedef struct
{
	individual ind[maxpop], /*Different Individuals*/
		* ind_ptr;
}population;             /*Popuation Structure*/

int iteration[times][7];
int conv[times][Gen];
int viola[times][Gen];



//#include "init_dispatch.h"         /*Random Initialization of the population*/
//
#include "InitBoarding.h" 
//
#include "sorting.h"      /*File Creating the Pareto Fronts*/
//
#include "sorting_fitness.h"

#include "fitness.h"     /*File Having the Function*/

#include "GWO_process.h"

#include "SCA_process.h"

#include "new_HS_process.h"




using namespace std;

population
old_pop,
new_pop,
new1_pop,
* old_pop_ptr,
* new_pop_ptr,
* new1_pop_ptr;
/*Defining the population Structures*/


void Read_Dispatch_Instance()
{
	//int Volume_Instance[MaxBusStopNum * MaxBusStopNum] = {};
	int count = 0;
	int k, b;// genes_dispatch[MaxTimeInt][MaxBusNum];
	if ((err = fopen_s(&fp, "Bus Dispatching.csv", "r")) != 0)
		printf("\nThere is no Instance file or no data in Instance\n");
	else
	{
		for (k = 0; k < BusStopNum; k++)
		{
			for (b = 0; b < BusNum; b++)
			{
				fscanf_s(fp, "%d", &DISPATCH[k][b]);
				//Volume_Instance[count] = StopVolume[0][i][j];
			}

		}
	}
	fclose(fp);

}


void Read_Boarding_Instance()
{
	//int Volume_Instance[MaxBusStopNum * MaxBusStopNum] = {};
	int count = 0;
	int k, b, i, j;// BoardVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];
	if ((err = fopen_s(&fp, "Bus Boarding.csv", "r")) != 0)
		printf("\nThere is no Instance file or no data in Instance\n");
	else
	{
		for (k = 0; k < BusStopNum; k++)
		{
			for (b = 0; b < BusNum; b++)
			{
				for (i = 0; i < BusStopNum; i++)
				{
					if (i == 0)
					{
						for (j = 1; j < BusStopNum; j++)
						{
							fscanf_s(fp, "%d", &BOARD_VOLUME[k][b][i][j]);
                        }
					}
					else
					{
						for (j = i+1; j < (BusStopNum+1); j++)
						{
							fscanf_s(fp, "%d", &BOARD_VOLUME[k][b][i][j]);
						}
					}
				}
				
				//Volume_Instance[count] = StopVolume[0][i][j];
			}

		}
	}
	fclose(fp);

}



void Generate_Passenger_Instance()
{
	int i, l, r;

	outfile.open("Instance.csv", ios::app);
	for (l = 0; l < BusStopNum; l++)
	{
		if (l == 0)
		{
			for (i = l + 1; i < (BusStopNum); i++)
			{
				r = rand() % 20;
				outfile << r << " ";
			}
			outfile << endl;
		}
		else
		{
			for (i = l + 1; i < (BusStopNum + 1); i++)
			{
				r = rand() % 20;
				outfile << r << " ";
			}
			outfile << endl;
		}
	}
	outfile << "/////////////////////////////////////" << endl;
	outfile.close();
}

void Read_Passenger_Instance()
{

	//fp = fopen("Instance.csv","r");
	//if(fp == NULL)
	//{
	//	printf("\nThere is no Proportion file or no data in destination_proportion2\n");
	//	system("pause");	
	//}
	// 
	//
	//	for(int j=0;j<StopNum;j++)
	//	{
	//		for(int i=0;i<StopNum;i++)
	//		{
	//         fscanf(fp,"%d",&PassStop[0][j][i]);
	//		}
	//	}
	//	fclose(fp);
	int Volume_Instance[MaxBusStopNum * MaxBusStopNum] = {};
	int count = 0;

	if ((err = fopen_s(&fp, "Instance.csv", "r")) != 0)
		printf("\nThere is no Instance file or no data in Instance\n");
	else
	{
		for (int i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (int j = i + 1; j < BusStopNum; j++)
				{
					//int file_input;
					fscanf_s(fp, "%d", &StopVolume[0][i][j]);
					Volume_Instance[count] = StopVolume[0][i][j];
					count = count + 1;
					//printf("%d\n", PassStop[0][j][i]);
				}
			}
			else
			{
				for (int j = i + 1; j < (BusStopNum + 1); j++)
				{
					//int file_input;
					fscanf_s(fp, "%d", &StopVolume[0][i][j]);
					Volume_Instance[count] = StopVolume[0][i][j];
					count = count + 1;
					//printf("%d\n", PassStop[0][j][i]);
				}
			}
		}

	}
	fclose(fp);

	// Instance for matlab
	outfile.open("Instance_matlab.csv", ios::app);
	for (int l = 0; l < count; l++)
	{
		outfile << Volume_Instance[l] << endl;
	}
	outfile.close();

}

void Generate_Proportions()
{
	int i, j, k, l, r;

	outfile.open("Proportion1.csv", ios::app);
	for (k = 0; k < Num_Int; k++)
	{
		for (l = 0; l < BusStopNum; l++)
		{
			r = rand() % 50;
			outfile << r << " ";
		}
		outfile << endl;
	}
	outfile << "/////////////////////////////////////" << endl;
	outfile.close();
	// averagely distributed
	//outfile.open("Proportion2.csv",ios::app);
	//	for(k=0;k<TripNum;k++)
	//	{
	//			for(l=0;l<StopNum;l++)
	//			{
	//				r=1;
	//				outfile<<r<<" ";
	//			}
	//			outfile<<endl;
	//	   }
	//outfile<<"/////////////////////////////////////"<<endl;
	//outfile.close();

	outfile.open("destination_proportion1.csv", ios::app);
	for (k = 0; k < Num_Int; k++)
	{
		for (l = 0; l < BusStopNum; l++)
		{
			if (l == 0)
			{
				for (i = l + 1; i < BusStopNum; i++)
				{
					r = rand() % 10;
					outfile << r << " ";
				}
				outfile << endl;
			}
			else
			{
				for (i = l + 1; i < (BusStopNum + 1); i++)
				{
					r = rand() % 10;
					outfile << r << " ";
				}
				outfile << endl;
			}
		}
		outfile << endl;
	}
	outfile << "/////////////////////////////////////" << endl;
	outfile.close();
	// // averagely distributed
	//outfile.open("destination_proportion2.csv",ios::app);
	//for(k=0;k<TripNum;k++)
	//{
	//		for(l=0;l<StopNum;l++)
	//		{
	//			for(i=0;i<StopNum;i++)
	//			{						
	//					r= 1;
	//					if(i==l)
	//					r=0;
	//					if((l>1) && (i<l)&&(i!=0))
	//					{
	//						r=0;
	//					}
	//					outfile<<r<<" ";				
	//			}
	//			outfile<<r<<endl;
	//		}
	//		outfile<<endl;
	//	}
	//outfile<<"/////////////////////////////////////"<<endl;
	//outfile.close();
}
//int desPro1[TripNum*StopNum*StopNum]={};
//int desPro2[TripNum*StopNum*StopNum]={};
void Read_Proportions()
{
	int i, j, k, l;

	string str;
	//fp = fopen("Proportion1.csv","r");
	//if(fp == NULL)
	//{
	//	printf("\nThere is no Proportion file or no data in Proportion1\n");
	//	system("pause");	
	//}
	//for(i=0;i<TripNum;i++)
	//	{
	//		for(j=0;j<StopNum;j++)
	//         {
	//			 fscanf(fp,"%d",&VolumeProportion1[i][j]);
	//         }
 //        }
	//fclose(fp);
	if ((err = fopen_s(&fp, "Proportion1.csv", "r")) != 0)
		printf("\nThere is no Proportion file or no data in Proportion1\n");
	else
	{
		for (int j = 0; j < Num_Int; j++)
		{
			for (int i = 0; i < BusStopNum; i++)
			{
				//int file_input;
				fscanf_s(fp, "%lf", &VolumeProportion1[j][i]);
				// printf("%d\n", VolumeProportion1[j][i]);
			}
		}

	}
	fclose(fp);

	//fp = fopen("Proportion2.csv","r");
	//if(fp == NULL)
	//{
	//	printf("\nThere is no Proportion file or no data in Proportion2\n");
	//	system("pause");	
	//}
	//	for(i=0;i<TripNum;i++)
	//	{
	//		for(j=0;j<StopNum;j++)
	//         {fscanf(fp,"%d",&VolumeProportion2[i][j]);
	//         }
 //        }
	//fclose(fp);

	//fp = fopen("destination_proportion1.csv","r");
	//if(fp == NULL)
	//{
	//	printf("\nThere is no Proportion file or no data in destination_proportion1\n");
	//	system("pause");	
	//}
	//	for(k=0;k<TripNum;k++)
	//	{
	//		for(l=0;l<StopNum;l++)
	//		{
	//			for(i=0;i<StopNum;i++)
	//			{
	//				fscanf(fp,"%d",&DestinationProportion1[k][l][i]); 
	//			}
	//		}
	//	}
	//fclose(fp);
	if ((err = fopen_s(&fp, "destination_proportion1.csv", "r")) != 0)
		printf("\nThere is no destination Proportion file or no data in destination_proportion1.csv\n");
	else
	{
		for (k = 0; k < Num_Int; k++)
		{
			for (int j = 0; j < BusStopNum; j++)
			{
				if (j == 0)
				{
					for (int i = j + 1; i < BusStopNum; i++)
					{

						fscanf_s(fp, "%lf", &DestinationProportion1[k][j][i]);
						//printf("%d\n", DestinationProportion1[k][j][i]);
					}
				}
				else
				{
					for (int i = j + 1; i < (BusStopNum + 1); i++)
					{

						fscanf_s(fp, "%lf", &DestinationProportion1[k][j][i]);
						//printf("%d\n", DestinationProportion1[k][j][i]);
					}
				}
			}
		}

	}
	fclose(fp);
	//fp = fopen("destination_proportion2.csv","r");
	//if(fp == NULL)
	//{
	//	printf("\nThere is no Proportion file or no data in destination_proportion2\n");
	//	system("pause");	
	//}
	//for(i=0;i<TripNum*StopNum*StopNum;i++)
	//{fscanf(fp,"%d",&desPro1[i]); }
	//fclose(fp);

}


void system_initialization()
{
	int Volume_FlowIn[MaxTimeInt * MaxBusNum * MaxBusStopNum] = {};
	int count = 0;

	double TotalProportion1[MaxTimeInt] = {};
	//int TotalProportion2[TripNum] ={};
	double TotalDesProportion1[MaxTimeInt][MaxBusStopNum] = {};
	for (int k = 0; k < Num_Int; k++)
	{
		for (int i = 0; i < BusStopNum; i++)
		{
			TotalProportion1[k] = TotalProportion1[k] + VolumeProportion1[k][i];
			//TotalProportion2[k]= TotalProportion2[k] +VolumeProportion2[k][i];
			if (i == 0)
			{
				for (int j = i + 1; j < BusStopNum; j++)
				{
					TotalDesProportion1[k][i] = TotalDesProportion1[k][i] + DestinationProportion1[k][i][j];
				}
			}
			else
			{
				for (int j = i + 1; j < (BusStopNum + 1); j++)
				{
					TotalDesProportion1[k][i] = TotalDesProportion1[k][i] + DestinationProportion1[k][i][j];
				}
			}
		}
	}

	for (int k = 0; k < Num_Int; k++)
	{
		for (int i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (int j = i + 1; j < BusStopNum; j++)
				{
					if ((TotalProportion1[k] == 0) || (TotalDesProportion1[k][i] == 0))
						FlowIn[k][i][j] = 0;
					else
						FlowIn[k][i][j] = (VolumeProportion1[k][i] / TotalProportion1[k]) * (DestinationProportion1[k][i][j] / TotalDesProportion1[k][i]) * Passenger_come;

					Volume_FlowIn[count] = FlowIn[k][i][j];
					count = count + 1;
				}
			}
			else
			{
				for (int j = i + 1; j < (BusStopNum + 1); j++)
				{
					if ((TotalProportion1[k] == 0) || (TotalDesProportion1[k][i] == 0))
						FlowIn[k][i][j] = 0;
					else
						FlowIn[k][i][j] = (VolumeProportion1[k][i] / TotalProportion1[k]) * (DestinationProportion1[k][i][j] / TotalDesProportion1[k][i]) * Passenger_come;

					Volume_FlowIn[count] = FlowIn[k][i][j];
					count = count + 1;
				}
			}

		}
	}
	// output the final value
	outfile.open("FlowIn_matlab.csv", ios::app);
	int k;
	for (k = 0; k < count; k++)
	{
		outfile << Volume_FlowIn[k] << endl;
	}
	outfile.close();

}
//void system_initialization()
//{
//	int i,j,k;
//	// stop volume initialization
// 	for(i=0; i< BusStopNum; i++)
//	{
//		if(i==0)
//		{
//			for(j=0; j<BusStopNum; j++)
//			{
//				StopVolume[0][i][j] = 10;
//				StopVolume[0][i][j] = 10;
//			}
//		}
//		else
//		{
//			for(j=i+1; j<(BusStopNum+1); j++)
//			{
//				StopVolume[0][i][j] = 10;
//				StopVolume[0][i][j] = 10;
//			}
//		}
//	}
//   
//	// incoming flow from outside
//	for(k=0; k<Num_Int; k++)
//	{
//		for(i=0; i< BusStopNum; i++)
//		{
//			for(j=0; j<(BusStopNum+1); j++)
//			{
//				FlowIn[k][i][j] = 10;
//
//			}
//		}
//	}
//
//}

int main()
{
	
	int k, b, i, j, l, m;
	int itc1, itc2, pp;

	int count;
	int Best_genes_dispatch[MaxTimeInt][MaxBusNum], Best_boarding[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];
	int termination1, termination2;
	int num, iteration1;
	double AA, BW, CC, BW_min, BW_max;
	double sum_time = 0;

	BW_min = 10;
	BW_max = 10000000;
	termination1 = 10;
	termination2 = 10;
	Passenger_come = 1000;
	//Generate_Passenger_Instance();
	Read_Passenger_Instance();
	//Generate_Proportions();
	Read_Proportions();


	//Read_Dispatch_Instance();
	//Read_Boarding_Instance();



	system_initialization();
	old_pop_ptr = &(old_pop);
	new_pop_ptr = &(new_pop);
	


	for (m = 0; m < times; m++)
	{

		time_t start = clock();

		srand(/*(unsigned)time(NULL) + */m*MaxBusNum * MaxBusStopNum * 1000000);




		        InitBoarding(old_pop_ptr);
				fitness_cost(old_pop_ptr, 1);
				sorting(old_pop_ptr, popsize, 1);
			

				for (iteration1 = 0; iteration1 < Gen; iteration1++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

					cout << old_pop_ptr->ind_ptr->fitness << "....." << "violations: =" << old_pop_ptr->ind_ptr->Constraint_1_viol << "..." << old_pop_ptr->ind_ptr->Constraint_2_viol << "..." << old_pop_ptr->ind_ptr->Constraint_3_viol << "..." << old_pop_ptr->ind_ptr->Constraint_4_viol << "..." << old_pop_ptr->ind_ptr->Constraint_5_viol << endl;
					
					
					AA = 2 * (1 - (double)(iteration1) / (double)(Gen)); 

					
					
					new_HS_process(old_pop_ptr, new_pop_ptr, AA);
					//GWO_process(old_pop_ptr, new_pop_ptr, AA);
					//SCA_process(old_pop_ptr, new_pop_ptr, AA);



					fitness_cost(old_pop_ptr, 2);
					sorting(old_pop_ptr, popsize, 2);

					count = 0;
					for (pp = 0; pp < 2 * popsize; pp++)
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[pp]);
						if (old_pop_ptr->ind_ptr->total_viol == 0)
							count = count + 1;
					}
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);
					
					if (count > 1)
						sorting_fitness(old_pop_ptr, popsize, count);

					

				conv[m][iteration1] = old_pop_ptr->ind_ptr->fitness;

				outfile.open("converge_curve.csv", ios::app);
				outfile << conv[m][iteration1] << endl;
				outfile.close();





				viola[m][iteration1] = old_pop_ptr->ind_ptr->total_viol;

				outfile.open("violation_curve.csv", ios::app);
				outfile << viola[m][iteration1] << endl;
				outfile.close();



				}

				time_t end = clock();

				double elapsed = (double)(end - start) / (CLOCKS_PER_SEC);
				sum_time = sum_time + elapsed;

				cout << "...................." << endl;
				cout << elapsed << endl;;
				cout << "...................." << endl;


				iteration[m][0] = old_pop_ptr->ind_ptr->fitness;
				iteration[m][1] = old_pop_ptr->ind_ptr->Acdelay;
				iteration[m][2] = old_pop_ptr->ind_ptr->Pedelay;
				iteration[m][3] = old_pop_ptr->ind_ptr->space;
				iteration[m][4] = old_pop_ptr->ind_ptr->total_viol;

				
				outfile.open("fitness.csv", ios::app);
				outfile << iteration[m][0] << endl;
				outfile.close();

				
				outfile.open("Delay1.csv", ios::app);
				outfile << iteration[m][1] << endl;
				outfile.close();
				
					outfile.open("Delay2.csv", ios::app);
					outfile << iteration[m][2] << endl;
					outfile.close();
				
				outfile.open("Space.csv", ios::app);
				outfile << iteration[m][3] << endl;
				outfile.close();

				
				outfile.open("Violation.csv", ios::app);
				outfile << iteration[m][4] << endl;
				outfile.close();






			/*	old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

				if (num == 0 && itc1 == 0)
				{
					Best_fitness = old_pop_ptr->ind_ptr->fitness;
					Best_violation = old_pop_ptr->ind_ptr->total_viol;
					for (k = 0; k < Num_Int; k++)
					{

						for (b = 0; b < BusNum; b++)
						{
							Best_genes_dispatch[k][b] = old_pop_ptr->ind_ptr->genes_dispatch[k][b];

							for (i = 0; i < BusStopNum; i++)
							{
								if (i == 0)
								{
									for (j = 1; j < BusStopNum; j++)
									{
										Best_boarding[k][b][i][j] = old_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

									}
								}
								else
								{
									for (j = i + 1; j < (BusStopNum + 1); j++)
									{
										Best_boarding[k][b][i][j] = old_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
									//	if (i == 5 && (j==7 && k == 0))
									//		cout << Best_boarding[k][b][i][j] << "...";
									}
									
								}

							}
							//cout << endl;
						}
					}


				}
				else
				{
					if (old_pop_ptr->ind_ptr->fitness < Best_fitness)
					{
						Best_fitness = old_pop_ptr->ind_ptr->fitness;
						Best_violation = old_pop_ptr->ind_ptr->total_viol;
						for (k = 0; k < Num_Int; k++)
						{

							for (b = 0; b < BusNum; b++)
							{
								Best_genes_dispatch[k][b] = old_pop_ptr->ind_ptr->genes_dispatch[k][b];

								for (i = 0; i < BusStopNum; i++)
								{
									if (i == 0)
									{
										for (j = 1; j < BusStopNum; j++)
										{
											Best_boarding[k][b][i][j] = old_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
										}
									}
									else
									{
										for (j = i + 1; j < (BusStopNum + 1); j++)
										{
											Best_boarding[k][b][i][j] = old_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
										}
									}

								}
							}
						}
					}

				}

				cout << Best_fitness << "............."<< Best_violation<<endl;
				cout << "....................................." << endl;
				
			}
*/
			

			////////////////   update dispatch array       ////////
		/*	double HMCR = 0.95, PAR = 0.5;
			double ra, ra2;
			int ind, r;
			for (num = 0; num < popsize; num++)
			{
				for (k = 0; k < Num_Int; k++)
				{
					for (b = 0; b < BusNum; b++)
					{

						ra = ((double)rand()) / ((double)RAND_MAX);
						if (ra < HMCR)
						{
							ra2 = ((double)rand()) / ((double)RAND_MAX);
							if (ra2 < PAR)
							{
								ind = rand() % popsize;
								new1_pop_ptr->ind_ptr->genes_dispatch[k][b] = ((new1_pop_ptr->ind_ptr) + (ind % (popsize / 2)))->genes_dispatch[k][b];
							}
							else
							{
								new1_pop_ptr->ind_ptr->genes_dispatch[k][b] = (new1_pop_ptr->ind_ptr)->genes_dispatch[k][b];

							}
						}//HMCR		
						else
						{
							new1_pop_ptr->ind_ptr->genes_dispatch[k][b] = rand() % 2;
							// adjust dispatch
							if (k > 0)
							{
								for (r = 0; r < k; r++)
								{
									if (new1_pop_ptr->ind_ptr->genes_dispatch[r][b] == 1)
									{
										if (k < (r + returnHorizon))
											new1_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
									}

								}
							}

						}//else			
					}//k
				}//b

			// adjust dispatch
				for (b = 0; b < BusNum; b++)
				{
					for (k = 0; k < Num_Int; k++)
					{
						if (k > 0)
						{
							for (j = 0; j < k; j++)
							{
								if (new1_pop_ptr->ind_ptr->genes_dispatch[j][b] == 1)
								{
									if (k < (j + returnHorizon))
										new1_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
								}

							}
						}
					}
				}

			}
					
			
*/







	//	}
		






//	}


		//GWO_dispatch(old_pop_ptr, new_pop_ptr);


			

			//GWO_dispatch(old_pop_ptr, new_pop_ptr);
			//InitBoarding(old_pop_ptr);

			//fitness_cost(old_pop_ptr, 1);

			//sorting(old_pop_ptr, popsize, 1);


    //}

    
 	

		
	//	old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

		/* record every test's fitness, passenger delay and bus vacancy */
	/*	iteration[m][0] = new3_pop_ptr->ind_ptr->fitness;
		iteration[m][1] = new3_pop_ptr->ind_ptr->Acdelay;
		iteration[m][2] = new3_pop_ptr->ind_ptr->Pedelay;
		iteration[m][3] = new3_pop_ptr->ind_ptr->space;
		iteration[m][4] = new3_pop_ptr->ind_ptr->Constraint_1_viol;
		iteration[m][5] = new3_pop_ptr->ind_ptr->Constraint_2_viol;
		iteration[m][6] = new3_pop_ptr->ind_ptr->Constraint_3_viol;
    */

		//outfile.open("CPU time.csv", ios::app);
		//outfile << end - start << endl;
		//outfile.close();

		//outfile.open("Bus results.csv", ios::app);
		//outfile << iteration[m][0] << " " << iteration[m][1] << " " << iteration[m][2] << " " << iteration[m][3] << " " << iteration[m][4] << " " << iteration[m][5] << " " << iteration[m][6] << endl;
		//outfile.close();

		//outfile.open("Load Time results.csv",ios::app);
		//for(k=0;k<Num_Int;k++)
		//{ 	
		//	for(b=0;b<BusNum;b++)
		//	{
		//		for(i=0;i<BusStopNum;i++)
		//		{
		//			outfile<<old_pop_ptr->ind_ptr->DataLoadTime[k][b][i]<<" ";
		//		}
		//		outfile<<endl;
		//	}
		//	outfile<<endl;

		//}
		//outfile.close();

		//outfile.open("Dwell Time results.csv",ios::app);
		//for(k=0;k<Num_Int;k++)
		//{ 	
		//		for(i=0;i<BusStopNum;i++)
		//		{
		//			outfile<<old_pop_ptr->ind_ptr->DataTrueDwellTime[k][i]<<" ";
		//		}
		//		outfile<<endl;

		//}
		//outfile.close();
	}
	outfile.open("CPU time.csv", ios::app);
	outfile << sum_time / times << endl;
	outfile.close();

	return 0;
}



