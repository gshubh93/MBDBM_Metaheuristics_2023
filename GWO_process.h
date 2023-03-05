#include <math.h>       /* exp */
#include <cstdlib>
#include <cmath>

#define pi 3.14
#define HMCR 1.0
//#define PAR 0.3

void GWO_process(population* pop_ptr, population* new_pop_ptr, double AA)
{
    int i, j, k, k1, b, r;

    int value_boarding;
    int pp;
    double a = AA;
    double pos;

    double a1_boarding, a2_boarding, a3_boarding, a_boarding;
    double A1_boarding, A2_boarding, A3_boarding;
    double C1_boarding, C2_boarding, C3_boarding;
    double new_boarding1, new_boarding2, new_boarding3;
    double rand1_boarding, rand2_boarding, rand3_boarding;
    double D_alpha_boarding, D_beta_boarding, D_delta_boarding;

    int a1_dispatch, a2_dispatch, a3_dispatch, a_dispatch;
    double r_D;
    double ra, ra2, R1, R2, rr;
    int rand_index;
    int sum_B = 0;
    int ind, P1;

    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);
    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);


    for (pp = 0; pp < popsize; pp++)
    {
   
        for (k = 0; k < Num_Int; k++)
        {
            for (b = 0; b < BusNum; b++)
            {
               
                pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                a1_dispatch = pop_ptr->ind_ptr->genes_dispatch[k][b];
                pop_ptr->ind_ptr = &(pop_ptr->ind[1]);
                a2_dispatch = pop_ptr->ind_ptr->genes_dispatch[k][b];
                pop_ptr->ind_ptr = &(pop_ptr->ind[2]);
                a3_dispatch = pop_ptr->ind_ptr->genes_dispatch[k][b];

                pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                a_dispatch = pop_ptr->ind_ptr->genes_dispatch[k][b];

                r_D = ((double)rand()) / ((double)RAND_MAX);


                    if (r_D <= (double)(1) / (double)(3))
                        new_pop_ptr->ind_ptr->genes_dispatch[k][b] = a1_dispatch;
                    else if (r_D > (double)(1) / (double)(3) && r_D< (double)(2) / (double)(3))
                        new_pop_ptr->ind_ptr->genes_dispatch[k][b] = a2_dispatch;
                    else
                        new_pop_ptr->ind_ptr->genes_dispatch[k][b] = a3_dispatch;



                if (k > 0)
                {
                    for (int jj = 0; jj < k; jj++)
                    {
                        if (new_pop_ptr->ind_ptr->genes_dispatch[jj][b] == 1)
                        {
                            if (k <= (jj + returnHorizon))
                            {
                                new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
                            }
                        }
                    }
                }

            }
        }



            for (k = 0; k < Num_Int; k++)
            {
                for (b = 0; b < BusNum; b++)
                {

                    for (i = 0; i < BusStopNum; i++)
                    {
                        if (i == 0)
                        {
                            for (j = 1; j < BusStopNum; j++)
                            {
                                if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
                                    value_boarding = 0;
                                else
                                {

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                    a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[1]);
                                    a2_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[2]);
                                    a3_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                    a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    rand1_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A1_boarding = 2 * a * rand1_boarding - a;
                                    C1_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_alpha_boarding = abs(C1_boarding * a1_boarding - a_boarding);

                                    new_boarding1 = a1_boarding - A1_boarding * D_alpha_boarding;



                                    rand2_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A2_boarding = 2 * a * rand2_boarding - a;
                                    C2_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_beta_boarding = abs(C2_boarding * a2_boarding - a_boarding);

                                    new_boarding2 = a2_boarding - A2_boarding * D_beta_boarding;



                                    rand3_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A3_boarding = 2 * a * rand3_boarding - a;
                                    C3_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_delta_boarding = abs(C3_boarding * a3_boarding - a_boarding);

                                    new_boarding3 = a3_boarding - A3_boarding * D_delta_boarding;
                                    

                                    value_boarding = ((new_boarding1 + new_boarding2 + new_boarding3) / 3);

                                    
                                }

                                pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                if ((value_boarding < 0) || (value_boarding > MaxBusSpace))
                                    value_boarding = a_boarding;

                                new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = (int)(value_boarding);

                            }
                        }
                        else
                        {
                            for (j = i + 1; j < (BusStopNum + 1); j++)
                            {
                                if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
                                    value_boarding = 0;
                                else
                                {

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                    a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[1]);
                                    a2_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[2]);
                                    a3_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                    a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    rand1_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A1_boarding = 2 * a * rand1_boarding - a;
                                    C1_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_alpha_boarding = abs(C1_boarding * a1_boarding - a_boarding);

                                    new_boarding1 = a1_boarding - A1_boarding * D_alpha_boarding;



                                    rand2_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A2_boarding = 2 * a * rand2_boarding - a;
                                    C2_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_beta_boarding = abs(C2_boarding * a2_boarding - a_boarding);

                                    new_boarding2 = a2_boarding - A2_boarding * D_beta_boarding;



                                    rand3_boarding = ((double)rand()) / ((double)RAND_MAX);
                                    A3_boarding = 2 * a * rand3_boarding - a;
                                    C3_boarding = 2 * (((double)rand()) / ((double)RAND_MAX));


                                    D_delta_boarding = abs(C3_boarding * a3_boarding - a_boarding);

                                    new_boarding3 = a3_boarding - A3_boarding * D_delta_boarding;


                                    value_boarding = ((new_boarding1 + new_boarding2 + new_boarding3) / 3);


                                }

                                pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                if ((value_boarding < 0) || (value_boarding > MaxBusSpace))
                                    value_boarding = a_boarding;

                                new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = (int)(value_boarding);
                            }
                        }

                    }

                }
            }

 
        // update the pointer
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[pp + 1]);
        pop_ptr->ind_ptr = &(pop_ptr->ind[pp + 1]);

    }//popsize


//////////////////////////////////   MERGE NEW POP TO OLD POP        ////////////////////////////////////////


    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);

    pop_ptr->ind_ptr = &(pop_ptr->ind[popsize]); // add child offspring after original pop (old pop)

    for (pp = 0; pp < popsize; pp++)
    {
        for (k = 0; k < Num_Int; k++)
        {
            for (b = 0; b < BusNum; b++)
            {

                pop_ptr->ind_ptr->genes_dispatch[k][b] = new_pop_ptr->ind_ptr->genes_dispatch[k][b];


                for (i = 0; i < BusStopNum; i++)
                {

                    if (i == 0)
                    {
                        for (j = 1; j < BusStopNum; j++)
                        {
                            pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                        }
                    }

                    else
                    {
                        for (j = i + 1; j < (BusStopNum + 1); j++)
                        {
                            pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                        }
                    }
                }
            }
        }
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[pp + 1]);
        pop_ptr->ind_ptr = &(pop_ptr->ind[popsize + pp + 1]);

    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);
}















