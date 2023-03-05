/*File to initialie the bus dispatch indicator decision variables:   x_b(k)*/
void InitBoarding(population* pop_ptr);//, population* new2_pop_ptr, int num1);

void InitBoarding(population* pop_ptr)//, population* new2_pop_ptr, int num1)
{
	int k, k1, b,i,j, pp;
	int sum_B;
	int val;
	int RR;
	int R1;
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
//	new2_pop_ptr->ind_ptr = &(new2_pop_ptr->ind[num1]);

	sum_B = 0;

	/*Loop Over the population size*/
	for (pp = 0; pp < popsize; pp++)
	{
		for (k = 0; k < Num_Int; k++)
		{
			for (b = 0; b < BusNum; b++)
			{
				pop_ptr->ind_ptr->genes_dispatch[k][b] = rand() % 2;

	/*			if (k > 0)
				{
					for (int jj = 0; jj < k; jj++)
					{
						if (pop_ptr->ind_ptr->genes_dispatch[jj][b] == 1)
						{
							if (k <= (jj + returnHorizon))
							{
								pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
							}
						}
					}
				}
		*/		
			}
		}





			
/*			RR = 2;// ceil((double)(Num_Int) / (double)(returnHorizon + 1));
			//cout << RR << ".....," << endl;
            
			if (RR == 1)
			{
				R1 = rand() % Num_Int;

				for (k = 0; k < Num_Int; k++)
				{
					if (k == R1)
						pop_ptr->ind_ptr->genes_dispatch[k][b] = 1;
					else
						pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
				}
			}
			else
			{
				int num = Num_Int - (returnHorizon + 1);
				int rrr = rand() % num;
				int rr = (returnHorizon+1 + rrr) + rand() % (Num_Int - (returnHorizon +1 + rrr));
				for (k = 0; k < Num_Int; k++)
				{
					//if (k == rrr || k == rr)
					pop_ptr->ind_ptr->genes_dispatch[k][b] = rand() % 2;
					//else
						//pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;

				}

			}
		}
*/
		/*Loop over the chromosome length*/
		for (k = 0; k < Num_Int; k++)
		{
			for (b = 0; b < BusNum; b++)
			{

				if (pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
				{
					for (i = 0; i < BusStopNum; i++)
					{
						if (i == 0)
						{
							for (j = 1; j < BusStopNum; j++)
							{
								pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = 0;
							}
						}
						else
						{
							for (j = i + 1; j < (BusStopNum + 1); j++)
							{
								pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = 0;
							}
						}
					}
				}
				else
				{
				
					for (i = 0; i < BusStopNum; i++)
					{
						if (i == 0)
						{
							for (j = 1; j < BusStopNum; j++)
							{
								pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = rand() % 100;
							}
						}
						else
						{
							for (j = i + 1; j < (BusStopNum + 1); j++)
							{
								pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = rand() % 100;
							}
						}
					}





				}
			}
		}

		pop_ptr->ind_ptr = &(pop_ptr->ind[pp + 1]);
	}

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

}
