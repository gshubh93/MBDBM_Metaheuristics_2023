#include <math.h>       /* exp */
#include <cstdlib>
#include <cmath>

#define pi 3.14
#define HMCR 1.0
#define PAR 1.0

void new_HS_process(population* pop_ptr, population* new_pop_ptr, double AA)
{
    int i, j, k, k1, b, r;

    int value_boarding;
    int pp;
    // int a1_boarding, a_boarding;
    double pos;

    double a1_boarding, a2_boarding, a3_boarding, a_boarding;
    double A1_boarding, A2_boarding, A3_boarding;
    double C1_boarding, C2_boarding, C3_boarding;
    double new_boarding1, new_boarding2, new_boarding3;
    double rand1_boarding, rand2_boarding, rand3_boarding;
    double D_alpha_boarding, D_beta_boarding, D_delta_boarding;

    double ra, ra2, R1, R2, rr;
    int rand_index;
    int sum_B = 0;
    int ind, P1;
    int rand_D;
    double C3;


    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);
    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);


    for (pp = 0; pp < popsize; pp++)
    {

        for (k = 0; k < Num_Int; k++)
        {
            for (b = 0; b < BusNum; b++)
            {
                rand_D = rand() % popsize;
                C3 = (((double)rand()) / ((double)RAND_MAX));

                if (C3 < HMCR)
                {
                    pop_ptr->ind_ptr = &(pop_ptr->ind[rand_D]);

                    new_pop_ptr->ind_ptr->genes_dispatch[k][b] = pop_ptr->ind_ptr->genes_dispatch[k][b];

                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);



                    double C4 = (((double)rand()) / ((double)RAND_MAX));
                    double F = (AA / 2);
                    if (C4 < F)
                    {
                        if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 1)   
                            new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
                        else
                            new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 1;
                    }
                }
                else
                {
                    new_pop_ptr->ind_ptr->genes_dispatch[k][b] = rand() % 2;
                }

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

                                int P1, P2, P3;
                                P1 = rand() % popsize;

                                do
                                {
                                    P2 = rand() % popsize;
                                } while (P2 == P1);

                                do
                                {
                                    P3 = rand() % popsize;
                                } while (P3 == P1 || P3==P2);



                                pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);
                                a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[P2]);
                                a2_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[P3]);
                                a3_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                double a11_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                double a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];


                                R1 = (((double)rand()) / ((double)RAND_MAX));

                                if (R1 < HMCR)
                                {
                                    //P1 = rand() % popsize;
                                    //pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);

                                    value_boarding = a1_boarding;  // pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    R2 = (((double)rand()) / ((double)RAND_MAX));

                                    if (R2 < PAR)
                                    {
                                       // rr = (((double)rand()) / ((double)RAND_MAX));
                                        //double C1 = 2 * (((double)rand()) / ((double)RAND_MAX));
                                        double C2 = (((double)rand()) / ((double)RAND_MAX));
                                        //  double AA_GWO = 2 * AA * rr - AA;

                                          //value_boarding = value_boarding + AA*(a11_boarding - value_boarding);

                                        value_boarding = value_boarding + (double)(AA/2) * (a2_boarding - a3_boarding);
                                        /*    if (rr <= 0.5)
                                                value_boarding = a11_boarding + AA * sin(2*pi*C2) * (C1 * a11_boarding - a_boarding);
                                            else
                                                value_boarding = a11_boarding + AA * cos(2 * pi * C2) * (C1 * a11_boarding - a_boarding);
                                        */
                                    }

                                }
                                else
                                {
                                    value_boarding = a_boarding;// rand() % 100;

                                }


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


                                int P1, P2, P3;
                                P1 = rand() % popsize;

                                do
                                {
                                    P2 = rand() % popsize;
                                } while (P2 == P1);

                                do
                                {
                                    P3 = rand() % popsize;
                                } while (P3 == P1 || P3 == P2);



                                pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);
                                a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[P2]);
                                a2_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[P3]);
                                a3_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                double a11_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                double a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];


                                R1 = (((double)rand()) / ((double)RAND_MAX));

                                if (R1 < HMCR)
                                {
                                    //P1 = rand() % popsize;
                                    //pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);

                                    value_boarding = a1_boarding; // pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                                    R2 = (((double)rand()) / ((double)RAND_MAX));

                                    if (R2 < PAR)
                                    {
                                        //rr = (((double)rand()) / ((double)RAND_MAX));
                                        //double C1 = 2 * (((double)rand()) / ((double)RAND_MAX));
                                        double C2 = (((double)rand()) / ((double)RAND_MAX));
                                        //  double AA_GWO = 2 * AA * rr - AA;

                                          //value_boarding = value_boarding + AA*(a11_boarding - value_boarding);

                                        value_boarding = value_boarding + (double)(AA / 2) * (a2_boarding - a3_boarding);
                                        /*    if (rr <= 0.5)
                                                value_boarding = a11_boarding + AA * sin(2*pi*C2) * (C1 * a11_boarding - a_boarding);
                                            else
                                                value_boarding = a11_boarding + AA * cos(2 * pi * C2) * (C1 * a11_boarding - a_boarding);
                                        */
                                    }

                                }
                                else
                                {
                                    value_boarding = a_boarding;// rand() % 100;

                                }


                            }

                            pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                            a_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];

                            if ((value_boarding < 0) || (value_boarding > MaxBusSpace))
                                value_boarding = a_boarding;

                            new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j] = (int)(value_boarding);
                        }
                    }


                }



                /////////////////////////    adjust dispatch and boarding         //////////////////////

                              /*      if (k > 0)
                                    {
                                        for (int jj = 0; jj < k; jj++)
                                        {
                                            if (new_pop_ptr->ind_ptr->genes_dispatch[jj][b] == 1)
                                            {
                                                if (k < (jj + returnHorizon))
                                                {
                                                    new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
                                                }
                                            }
                                        }
                                    }

                                    if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
                                    {
                                        for (i = 0; i < BusStopNum; i++)
                                        {
                                            if (i == 0)
                                            {
                                                for (j = 1; j < BusStopNum; j++)
                                                {
                                                    new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j] == 0;
                                                }
                                            }
                                            else
                                            {
                                                for (j = i + 1; j < (BusStopNum + 1); j++)
                                                {
                                                    new_pop_ptr->ind_ptr->BoardVolume[k][b][i][j] == 0;
                                                }
                                            }
                                        }
                                    }
                */


                /*                    if (sum_B == 0)
                                        new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 0;
                                    else
                                        new_pop_ptr->ind_ptr->genes_dispatch[k][b] = 1;

                                    sum_B = 0;
                */



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















