/*This is the program used to evaluate the value of the function
************************************************************************/

// Bus Volume of bus b in trip k when it approaching stop i with destination stop j
int BusVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];

// Stop Volume at stop i when bus b of trip k is coming
int StopVolume[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];

// Total Volume in bus b of trip k when approaching stop j (do not include the alighting volume at stop j, \sum_{i}V[k][b][i][j] is not added in V[k][b][j])
int TotalBusVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum];

// Total Boarding Volume of bus b in trip k at stop i 
int TotalBoardVolume1[MaxTimeInt][MaxBusNum][MaxBusStopNum];
int TotalBoardVolume2[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];

// incoming passenger flow for bus stop i with destination stop j when buses of trip k is coming
extern int FlowIn[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];

// loading time for bus b of trip k at stop i 
int LoadTime[MaxTimeInt][MaxBusNum][MaxBusStopNum];

// Constraint vector corresponding to equation (3a) 
int CONS1[MaxTimeInt][MaxBusNum][MaxBusStopNum];

// Constraint vector corresponding to equation (3b) 
int CONS2[MaxTimeInt][MaxBusStopNum][MaxBusStopNum];

// Constraint vector corresponding to equation (8) 
int CONS3[MaxTimeInt][MaxBusNum][MaxBusStopNum];

int CONS4[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];

int CONS5[MaxTimeInt][MaxBusNum];

struct data {
	int Adelay; // Actual Delay
	int Pdelay; // Perceived Delay
	int space;  // Bus Vacancy
	int dataLoadTime[MaxTimeInt][MaxBusNum][MaxBusStopNum];
	//	int dataTrueDwellTime[MaxTimeInt][MaxBusStopNum];

	int Constraint1_VIOL;
	int Constraint2_VIOL;
	int Constraint3_VIOL;
	int Constraint4_VIOL;
	int Constraint5_VIOL;

	//int dataBusVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum];
	//int dataBoardVolume[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum]; 
};


struct data Calculate_Bus_Cost(int(&BusDispatch)[MaxTimeInt][MaxBusNum], int(&BoardingVolume)[MaxTimeInt][MaxBusNum][MaxBusStopNum][MaxBusStopNum])
{
	int i, j, k, b, r, m, n;
	int sum_b = 0;
	int AlreadyBoardVolume[MaxTimeInt][MaxBusStopNum][MaxBusStopNum] = {};
	int BoardVol = 0;
	int PassDelay = 0, PassDelay2 = 0, BusVacancy = 0;

	struct data returnvalue;

	int sumboardtime = 0;
	int sumalighttime = 0;
	int sum_horizon = 0;
	int sum_constraint1 = 0;
	int sum_constraint2 = 0;
	int sum_constraint3 = 0;
	int sum_constraint4 = 0;
	int sum_constraint5 = 0;

	int max_boarding[MaxBusNum][MaxBusStopNum][MaxBusStopNum] = {};
	int max_volume[MaxBusNum][MaxBusStopNum][MaxBusStopNum] = {};
	int max_dispatch[MaxBusNum] = {};

	for (k = 0; k < Num_Int; k++)
	{
		// start stop i 
		for (i = 0; i < BusStopNum; i++)
		{
			// calculate the bus volume when approaching stop i
			if (i == 0) // terminal stop
			{
				for (b = 0; b < BusNum; b++)
				{
					for (j = 1; j < BusStopNum; j++)
						TotalBusVolume[k][b][i] = TotalBusVolume[k][b][i] + BusVolume[k][b][i][j];
				}
			}
			else
			{
				for (b = 0; b < BusNum; b++)
				{
					for (j = i + 1; j < (BusStopNum + 1); j++)  // not include BusVolume[k][b][i][i], since they already left buses when reach stop i, which leads to vacancy for boarding passengers
						TotalBusVolume[k][b][i] = TotalBusVolume[k][b][i] + BusVolume[k][b][i][j];
				}
			}

			/*Boarding passengers over all destination stations for equation (3a) */

			if (i == 0) // terminal stop
			{
				for (b = 0; b < BusNum; b++)
				{
					for (j = 1; j < BusStopNum; j++)
						TotalBoardVolume1[k][b][i] = TotalBoardVolume1[k][b][i] + BoardingVolume[k][b][i][j];
				}
			}
			else
			{
				for (b = 0; b < BusNum; b++)
				{
					for (j = i + 1; j < (BusStopNum + 1); j++)  // not include BusVolume[k][b][i][i], since they already left buses when reach stop i, which leads to vacancy for boarding passengers
						TotalBoardVolume1[k][b][i] = TotalBoardVolume1[k][b][i] + BoardingVolume[k][b][i][j];
				}
			}


			/*Boarding passengers over all busses for equation (3b) */

			if (i == 0) // terminal stop
			{
				for (j = 1; j < BusStopNum; j++)
				{
					for (b = 0; b < BusNum; b++)
						TotalBoardVolume2[k][i][j] = TotalBoardVolume2[k][i][j] + BoardingVolume[k][b][i][j];
				}
			}
			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					for (b = 0; b < BusNum; b++)  // not include BusVolume[k][b][i][i], since they already left buses when reach stop i, which leads to vacancy for boarding passengers
						TotalBoardVolume2[k][i][j] = TotalBoardVolume2[k][i][j] + BoardingVolume[k][b][i][j];
				}
			}

			/**************************************************************************************/
			// Bus Volume update
			if (i == 0)
			{
				for (j = 1; j < BusStopNum; j++)
				{
					for (b = 0; b < BusNum; b++)
					{
						BusVolume[k][b][i + 1][j] = BusVolume[k][b][i][j] + BoardingVolume[k][b][i][j];
					}
				}
			}
			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					for (b = 0; b < BusNum; b++)
					{
						BusVolume[k][b][i + 1][j] = BusVolume[k][b][i][j] + BoardingVolume[k][b][i][j];
					}
				}
			}

			// loading time for bus b
			for (b = 0; b < BusNum; b++)
			{
				//	if (BusDispatch[k][b] == 1)
				//	{
				if (i == 0)
				{
					//	for (j = 1; j < BusStopNum; j++)
					//	{
					//		sumboardtime = sumboardtime + BoardTime * BoardingVolume[k][b][i][j];
					//	}

					LoadTime[k][b][i] = OpenTime + BoardTime * TotalBoardVolume1[k][b][i];// sumboardtime;

				}
				else
				{
					//for (j = i + 1; j < (BusStopNum + 1); j++)
					//{
					//	sumboardtime = sumboardtime + BoardTime * BoardingVolume[k][b][i][j];
					//}
					sumboardtime = BoardTime * TotalBoardVolume1[k][b][i];
					sumalighttime = AlightTime * BusVolume[k][b][i - 1][i];

					LoadTime[k][b][i] = OpenTime + max(sumboardtime, sumalighttime);
				}


				//	}
					//sumalighttime = 0;
					//sumboardtime = 0;

			}


			//} // stop i

			//Stop Volume update
			//for (i = 0; i < BusStopNum; i++)
			//{
			if (i == 0)
			{
				for (j = 1; j < (BusStopNum); j++)
				{
					//			for(b=0;b<BusNum;b++)
					//			{
					//				sum_b = sum_b + BoardVolume[k][b][i][j];
					//			}
					StopVolume[k + 1][i][j] = StopVolume[k][i][j] + FlowIn[k][i][j] - TotalBoardVolume2[k][i][j];// sum_b;
		//			sum_b=0;
				}
			}
			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					//			for(b=0;b<BusNum;b++)
					//			{
					//				sum_b = sum_b + BoardVolume[k][b][i][j];
					//			}
					StopVolume[k + 1][i][j] = StopVolume[k][i][j] + FlowIn[k][i][j] - TotalBoardVolume2[k][i][j]; //sum_b;
		//			sum_b=0;
				}
			}
		}

	}



	for (b = 0; b < BusNum; b++)
	{
		for (i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (j = 1; j < BusStopNum; j++)
				{	
					for (k = 0; k < Num_Int; k++)
					{
						max_boarding[b][i][j] = max(max_boarding[b][i][j], BoardingVolume[k][b][i][j]);
						max_volume[b][i][j] = max(max_volume[b][i][j], BusVolume[k][b][i][j]);
					}
				}
			}
			else
			{
				for (j = i+1; j < (BusStopNum+1); j++)
				{		
					for (k = 0; k < Num_Int; k++)
					{
						max_boarding[b][i][j] = max(max_boarding[b][i][j], BoardingVolume[k][b][i][j]);
						max_volume[b][i][j] = max(max(max_volume[b][i][j], BusVolume[k][b][i][j]), BusVolume[k][b][i-1][i]);
					}
				}
			}
		}
	}


	//int sum_horizon[MaxTimeInt][MaxBusNum] = {};
	

	for (k = 0; k < Num_Int; k++)
	{
		for (b = 0; b < BusNum; b++)
		{
			if ((k + returnHorizon-1) >= (Num_Int-1))
			{
				for (int k1 = 1; k1 <= (Num_Int-k); k1++)
				{
					sum_horizon = sum_horizon + BusDispatch[k + k1][b];
					//sum_horizon[k][b] = sum_horizon[k][b] + BusDispatch[k + k1][b];

				}
			}
			else
			{
				for (int k1 = 1; k1 <= (returnHorizon); k1++)
				{
					sum_horizon = sum_horizon + BusDispatch[k + k1][b];
					//sum_horizon[k][b] = sum_horizon[k][b] + BusDispatch[k + k1][b];

				}
			}

			CONS5[k][b] = sum_horizon - (1) * (1 - BusDispatch[k][b]);
			sum_horizon = 0;
		}
	}



	/*for (b = 0; b < BusNum; b++)
	{
		for (k = 0; k < Num_Int; k++)
		{
			max_dispatch[b] = max(max_dispatch[b], sum_horizon[k][b]);
		}
	}
	*/





	for (b = 0; b < BusNum; b++)
	{
		for (k = 0; k < Num_Int; k++)
		{

		//	CONS5[k][b] = sum_horizon[k][b] - (returnHorizon+1) * (1 - BusDispatch[k][b]);
		//	sum_horizon[k][b] = 0;

			for (i = 0; i < BusStopNum; i++)
			{
				CONS1[k][b][i] = TotalBoardVolume1[k][b][i] + TotalBusVolume[k][b][i] - MaxBusSpace;
				CONS3[k][b][i] = LoadTime[k][b][i] - DwellTime;

				if (i == 0)
				{
					for (j = 1; j < BusStopNum; j++)
					{
						CONS4[k][b][i][j] = BoardingVolume[k][b][i][j]- 100 *BusDispatch[k][b];
					}
				}
				else
				{
					for (j = i+1; j < (BusStopNum+1); j++)
					{
						CONS4[k][b][i][j] = BoardingVolume[k][b][i][j] -100 * BusDispatch[k][b];
					}
				}
			}
		}


		for (i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (j = 1; j < (BusStopNum); j++)
				{
					CONS2[k][i][j] = TotalBoardVolume2[k][i][j] - StopVolume[k][i][j];
				}
			}

			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					CONS2[k][i][j] = TotalBoardVolume2[k][i][j] - StopVolume[k][i][j];
				}
			}
		}

		/*	for (b = 0; b < BusNum; b++)
			{
				for (i = 0; i < BusStopNum; i++)
				{
					CONS3[k][b][i] = LoadTime[k][b][i] - DwellTime;
				}
			}
		*/

	}// trip k



	/*sum_constraint denotes the total violation occured in equations (3a), (3b) and (8)*/

	for (k = 0; k < Num_Int; k++)
	{
		/*  Constraint 3 (a)  SUM  */
		for (b = 0; b < BusNum; b++)
		{
			sum_constraint5 = sum_constraint5 + max(CONS5[k][b], 0);
			for (i = 0; i < BusStopNum; i++)
			{
				sum_constraint1 = sum_constraint1 + max(CONS1[k][b][i], 0);
				sum_constraint3 = sum_constraint3 + max(CONS3[k][b][i], 0);

				if (i == 0)
				{
					for (j = 1; j < BusStopNum; j++)
					{
						sum_constraint4 = sum_constraint4 + max(CONS4[k][b][i][j], 0);
					}
				}
				else
				{
					for (j = i + 1; j < (BusStopNum + 1); j++)
					{
						sum_constraint4 = sum_constraint4 + max(CONS4[k][b][i][j], 0);
					}
				}
			}
		}

		/*   Constraint 3 (b) SUM  */
		for (i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (j = 1; j < (BusStopNum); j++)
				{
					sum_constraint2 = sum_constraint2 + max(CONS2[k][i][j], 0);
				}
			}
			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					sum_constraint2 = sum_constraint2 + max(CONS2[k][i][j], 0);
				}
			}
		}

			/*   Constraint (8) SUM    */
		/*	for (b = 0; b < BusNum; b++)
			{
				for (i = 0; i < BusStopNum; i++)
				{
					sum_constraint3 = sum_constraint3 + max(CONS3[k][b][i],0);
				}
			}
		*/

	}

	/////////////////////////*********************///////////////// (DELAY)

	for (k = 0; k < Num_Int; k++)
	{
		for (i = 0; i < BusStopNum; i++)
		{
			if (i == 0)
			{
				for (j = 1; j < (BusStopNum); j++)
				{
					PassDelay = PassDelay + StopVolume[k][i][j];
				}
			}
			else
			{
				for (j = i + 1; j < (BusStopNum + 1); j++)
				{
					PassDelay = PassDelay + StopVolume[k][i][j];
				}
			}
		}
	}

	/////////////////////////*********************///////////////// (PERCEIVED DELAY)
	for (k = 0; k < Num_Int - 1; k++)
	{
		for (i = 0; i < BusStopNum; i++)
		{
			for (n = k + 1; n < Num_Int; n++)
			{
				if (i == 0)
				{
					for (j = 1; j < (BusStopNum); j++)
					{
						for (b = 0; b < BusNum; b++)
						{
							BoardVol = BoardVol + BoardingVolume[k][b][i][j];
							for (r = k + 1; r < n + 1; r++)
								AlreadyBoardVolume[n][i][j] = AlreadyBoardVolume[n][i][j] + BoardingVolume[r][b][i][j];
						}
						PassDelay2 = PassDelay2 + max(FlowIn[k][i][j] - max(AlreadyBoardVolume[n][i][j] - (StopVolume[k][i][j] - BoardVol), 0), 0) * square(n - (k + 1));
					}
					BoardVol = 0;
				}
				else
				{
					for (j = 1; j < (BusStopNum + 1); j++)
					{
						for (b = 0; b < BusNum; b++)
						{
							BoardVol = BoardVol + BoardingVolume[k][b][i][j];
							for (r = k + 1; r < n + 1; r++)
								AlreadyBoardVolume[n][i][j] = AlreadyBoardVolume[n][i][j] + BoardingVolume[r][b][i][j];
						}
						PassDelay2 = PassDelay2 + max(FlowIn[k][i][j] - max(AlreadyBoardVolume[n][i][j] - (StopVolume[k][i][j] - BoardVol), 0), 0) * square(n - (k + 1));
					}
					BoardVol = 0;
				}
			}
		}
	}
	
	/////////////////////////*********************///////////////// (REMAIN SPACE)
	int volumenum = 0;

	for (k = 0; k < Num_Int; k++)
	{
		for (b = 0; b < BusNum; b++)
		{
			for (i = 0; i < BusStopNum; i++)
			{
				if (i == 0)
				{
					//if(BusDispatch[k][b]==1) // only the dispatched buses consider the bus vacancy
					//{
					for (j = 0; j < (BusStopNum); j++) //include the alighting volume at stop i, \sum_{q}V[k][b][q][i] is considered in bus volume when bus leave stop i-1
					{
						volumenum = volumenum + BusVolume[k][b][i][j];
					}
					BusVacancy = BusVacancy + MaxBusSpace - volumenum;
					
					volumenum = 0;
					//}
				}
				else
				{
					//if(BusDispatch[k][b]==1) // only the dispatched buses consider the bus vacancy
					//{
					for (j = i; j < (BusStopNum + 1); j++) //include the alighting volume at stop i, \sum_{q}V[k][b][q][i] is considered in bus volume when bus leave stop i-1
					{
						volumenum = volumenum + BusVolume[k][b][i][j];
					}
					BusVacancy = BusVacancy + MaxBusSpace - volumenum;
					volumenum = 0;
					//}
				}
			}
		}
	}

	////////////////////////////////////////////////////
	for (k = 0; k < Num_Int; k++)
	{
		for (i = 0; i < BusStopNum; i++)
		{
			for (b = 0; b < BusNum; b++)
			{
				returnvalue.dataLoadTime[k][b][i] = LoadTime[k][b][i];
			}
			//			returnvalue.dataTrueDwellTime[k][i] = TrueDwellTime[k][i];
		}
	}

	//for(k=0;k<Num_Int;k++)
	//{
	//	for(b=0;b<BusNum;b++)
	//	{
	//		for(i=0;i<BusStopNum;i++)
	//		{
	//			if(i==0)
	//			{
	//				for(j=0;j<BusStopNum;j++)
	//				{
	//					returnvalue.dataBoardVolume[k][b][i][j] = BoardVolume[k][b][i][j];
	//					returnvalue.dataBusVolume[k][b][i][j] = BusVolume[k][b][i][j];
	//				}
	//			}
	//			else
	//			{
	//				for(j=0;j<BusStopNum+1;j++)
	//				{
	//					returnvalue.dataBoardVolume[k][b][i][j] = BoardVolume[k][b][i][j];
	//					returnvalue.dataBusVolume[k][b][i][j] = BusVolume[k][b][i][j];
	//				}
	//			}
	//		}
	//	}
	//}

	returnvalue.Adelay = PassDelay;
	returnvalue.Pdelay = PassDelay2;
	returnvalue.space = BusVacancy;

	returnvalue.Constraint1_VIOL = sum_constraint1;
	returnvalue.Constraint2_VIOL = sum_constraint2;
	returnvalue.Constraint3_VIOL = sum_constraint3;
	returnvalue.Constraint4_VIOL = sum_constraint4;
	returnvalue.Constraint5_VIOL = sum_constraint5;

	return returnvalue;
}

void fitness_cost(population* pop_ptr, int m)
{
	/*File ptr to the file to store the value of the g for last iteration
	  g is the parameter required for a particular problem
	  Every problem is not required*/

	int fit_delay1,      /* Passenger Actual delay */
		fit_delay2,	   /* Passenger Perceived delay */
		fit_space;      /* bus vacancy */

	int fit_CONS_VIOL1, fit_CONS_VIOL2, fit_CONS_VIOL3, fit_CONS_VIOL4, fit_CONS_VIOL5;

	int i, j, k, b, index_fit, index;
	if (m == 1)
		index_fit = 0;
	if (m == 2)
		index_fit = popsize;

	/*Initializing the max rank to zero*/
	for (index = 0; index < popsize; index++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[index + index_fit]);

		////////////////////////////////////////////////////////////////////
		/* calculate passenger delay and bus vacancy */
		/////////////////////////////////////////////////////////////////////
			// every time re-initial the data setting
		for (k = 0; k < (Num_Int + 1); k++)
		{
			for (b = 0; b < BusNum; b++)
			{
				for (i = 0; i < (BusStopNum + 1); i++)
				{
					for (j = 0; j < (BusStopNum + 1); j++)
					{
						BusVolume[k][b][i][j] = 0;
					}
					TotalBoardVolume1[k][b][i] = 0;
					TotalBusVolume[k][b][i] = 0;
					LoadTime[k][b][i] = 0;

				//	CONS1[k][b][i] = 0;
				//	CONS3[k][b][i] = 0;
				}
				//			TrueDwellTime[k][i] = 0;
			}
		}


		for (k = 0; k < (Num_Int + 1); k++)
		{
			for (i = 0; i < (BusStopNum + 1); i++)
			{
				for (j = 0; j < (BusStopNum + 1); j++)
				{
					if (k > 0)
						StopVolume[k][i][j] = 0;

					TotalBoardVolume2[k][i][j] = 0;
					//CONS2[k][i][j] = 0;
				}
			}
		}



		struct data totalcost;
		//delay cost
		totalcost = Calculate_Bus_Cost(pop_ptr->ind_ptr->genes_dispatch, pop_ptr->ind_ptr->BoardVolume);
		fit_delay1 = totalcost.Adelay;
		fit_delay2 = totalcost.Pdelay;
		fit_space = totalcost.space;

		fit_CONS_VIOL1 = totalcost.Constraint1_VIOL;
		fit_CONS_VIOL2 = totalcost.Constraint2_VIOL;
		fit_CONS_VIOL3 = totalcost.Constraint3_VIOL;
		fit_CONS_VIOL4 = totalcost.Constraint4_VIOL;
		fit_CONS_VIOL5 = totalcost.Constraint5_VIOL;

		//for(k=0;k<Num_Int;k++)
		//{
		//	for(b=0;b<BusNum;b++)
		//	{
		//		for(i=0;i<BusStopNum;i++)
		//		{
		//			if(i==0)
		//			{
		//				for(j=0;j<BusStopNum;j++)
		//				{
		//					pop_ptr->ind_ptr->boardvolume[k][b][i][j]=totalcost.dataBoardVolume[k][b][i][j];
		//					pop_ptr->ind_ptr->busvolume[k][b][i][j]=totalcost.dataBusVolume[k][b][i][j];
		//				}
		//			}
		//			else
		//			{
		//				for(j=0;j<BusStopNum+1;j++)
		//				{
		//					pop_ptr->ind_ptr->boardvolume[k][b][i][j]=totalcost.dataBoardVolume[k][b][i][j];
		//					pop_ptr->ind_ptr->busvolume[k][b][i][j]=totalcost.dataBusVolume[k][b][i][j];
		//				}
		//			}
		//		}
		//	}
		//}

		for (k = 0; k < (Num_Int + 1); k++)
		{
			for (i = 0; i < BusStopNum; i++)
			{
				for (b = 0; b < BusNum; b++)
				{
					pop_ptr->ind_ptr->DataLoadTime[k][b][i] = totalcost.dataLoadTime[k][b][i];
				}
				//			pop_ptr->ind_ptr->DataTrueDwellTime[k][i] = totalcost.dataTrueDwellTime[k][i];
			}
		}

		pop_ptr->ind_ptr->Constraint_1_viol = fit_CONS_VIOL1;
		pop_ptr->ind_ptr->Constraint_2_viol = fit_CONS_VIOL2;
		pop_ptr->ind_ptr->Constraint_3_viol = fit_CONS_VIOL3;
		pop_ptr->ind_ptr->Constraint_4_viol = fit_CONS_VIOL4;
		pop_ptr->ind_ptr->Constraint_5_viol = fit_CONS_VIOL5;
		pop_ptr->ind_ptr->total_viol = fit_CONS_VIOL1 + fit_CONS_VIOL2 + fit_CONS_VIOL3 + fit_CONS_VIOL4 + fit_CONS_VIOL5;
		pop_ptr->ind_ptr->fitness = fit_delay1 + fit_space;// +1000 * (fit_CONS_VIOL1 + fit_CONS_VIOL2 + fit_CONS_VIOL3 + fit_CONS_VIOL4 + fit_CONS_VIOL5);
		pop_ptr->ind_ptr->space = fit_space;
		pop_ptr->ind_ptr->Acdelay = fit_delay1;
		pop_ptr->ind_ptr->Pedelay = fit_delay2;

	}
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

}



