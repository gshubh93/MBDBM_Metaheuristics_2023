#include <math.h>       /* exp */
#include <cstdlib>
#include <cmath>

#define pi 3.14

void SCA_process(population* pop_ptr, population* new_pop_ptr, double AA)
{
    int i, j, k, k1, b, r;

    double value_boarding;
    int pp;
    int  a1_dispatch, a2_dispatch, a3_dispatch;
   // int a1_boarding, a_boarding;
    double pos;
    double a = AA;
    double a1_boarding, a2_boarding, a3_boarding, a_boarding;
    double A1_boarding, A2_boarding, A3_boarding;
    double C1_boarding, C2_boarding, C3_boarding;
    double new_boarding1, new_boarding2, new_boarding3;
    double rand1_boarding, rand2_boarding, rand3_boarding;
    double D_alpha_boarding, D_beta_boarding, D_delta_boarding;
    
    int index;
    int count = 0;


    double FF = AA;// / 2;// 0.5;
    double ra, ra2;
    int rand_index;
    int sum_B = 0;
    int ind;

    new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[0]);
    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);


    for (pp = 0; pp < popsize; pp++)
    {
       



        for (k = 0; k < Num_Int; k++)
        {
            for (b = 0; b < BusNum; b++)
            {
                int rand_D = rand() % popsize;
                double C3 = (((double)rand()) / ((double)RAND_MAX));

                if (C3<0.5)
                    new_pop_ptr->ind_ptr->genes_dispatch[k][b] = pop_ptr->ind_ptr->genes_dispatch[k][b];
                else
                {
                    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                    new_pop_ptr->ind_ptr->genes_dispatch[k][b] = pop_ptr->ind_ptr->genes_dispatch[k][b];
                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
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
							
							//if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
                            //        value_boarding = 0;
                            //    else
                            //    {
                                    
                            /*        for (int pp1 = 0; pp1 < popsize; pp1++)
                                    {
                                        pop_ptr->ind_ptr = &(pop_ptr->ind[pp1]);
                                        if (pop_ptr->ind_ptr->BoardVolume[k][b][i][j] > 0)
                                        {
                                            index = pp1;
                                            count = 1;
                                            break;
                                        }

                                    }

                                    count = 0;
                            */
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                    a_boarding = ((pop_ptr->ind_ptr) + 0)->BoardVolume[k][b][i][j];

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                    a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);

                                    double rr = (((double)rand()) / ((double)RAND_MAX));
                                    double C = 2*(((double)rand()) / ((double)RAND_MAX));
                                    double rr1 = (((double)rand()) / ((double)RAND_MAX));

                                    if (rr1 < 0.5)
                                    {
                                        value_boarding = a_boarding+AA * sin(2 * PI * rr) * abs(C * a1_boarding - a_boarding);
                                    }
                                    else
                                    {
                                        value_boarding = a_boarding+AA * cos(2 * PI * rr) * abs(C * a1_boarding - a_boarding);
                                    }

                                    /*
                                    double cross = (((double)rand()) / ((double)RAND_MAX));


                                    if (cross > 0.2)
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

                                        int RR = rand() % 3;
                                        if (RR==0)
                                        pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);
                                        else if (RR==1)
                                            pop_ptr->ind_ptr = &(pop_ptr->ind[P2]);
                                        else
                                            pop_ptr->ind_ptr = &(pop_ptr->ind[P3]);
                                        value_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    }
                              //  }
                              */
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
						   //     if (new_pop_ptr->ind_ptr->genes_dispatch[k][b] == 0)
                           //             value_boarding = 0;
                           //     else
                           //     {

                            /*        for (int pp1 = 0; pp1 < popsize; pp1++)
                                    {
                                        pop_ptr->ind_ptr = &(pop_ptr->ind[pp1]);

                                        if (pop_ptr->ind_ptr->BoardVolume[k][b][i][j] > 0)
                                        {
                                            index = pp1;
                                            count = 1;
                                            break;
                                        }

                                    }

                                    count = 0;
                            */
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);
                                    a_boarding = ((pop_ptr->ind_ptr) + 0)->BoardVolume[k][b][i][j];

                                    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
                                    a1_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                    pop_ptr->ind_ptr = &(pop_ptr->ind[pp]);

                                    double rr = (((double)rand()) / ((double)RAND_MAX));
                                    double C = 2*(((double)rand()) / ((double)RAND_MAX));
                                    double rr1 = (((double)rand()) / ((double)RAND_MAX));

                                    if (rr1 < 0.5)
                                    {
                                        value_boarding = a_boarding+AA * sin(2 * PI * rr) * abs(C * a1_boarding - a_boarding);
                                    }
                                    else
                                    {
                                        value_boarding = a_boarding+AA * cos(2 * PI * rr) * abs(C * a1_boarding - a_boarding);
                                    }

                                    /*
                                    double cross = (((double)rand()) / ((double)RAND_MAX));

                                        if (cross >0.2)
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

                                            int RR = rand() % 3;
                                            if (RR == 0)
                                                pop_ptr->ind_ptr = &(pop_ptr->ind[P1]);
                                            else if (RR == 1)
                                                pop_ptr->ind_ptr = &(pop_ptr->ind[P2]);
                                            else
                                                pop_ptr->ind_ptr = &(pop_ptr->ind[P3]);
                                            value_boarding = pop_ptr->ind_ptr->BoardVolume[k][b][i][j];
                                        }
                                          //  value_boarding = a_boarding;
                                    
                            //    }
                                    */
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















