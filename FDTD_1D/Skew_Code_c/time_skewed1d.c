/* FD1D_1.1.c.   1D FDTD simulation in free space
with absorbing conditions */

//CMD EVALUATIONS
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c

//HARD-CODED TIME STEP.

// L1 Threshold NZ: 4096 
// L3 Threshold NZ: 786432

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <string.h>

#define NZ  100000    /* NZ is the number of cells to be used */
//experimenting with the step size to see the changes at the cache performance

#define MAX(a,b) ( ( a > b) ? a : b ) 
#define MIN(a,b) ( ( a < b) ? a : b ) 

int main ()
{
	
    //2048
      
      const int s = 40; //2 for very small test default 1000//tile width 50000
	  
	  

	  
	  float H2[NZ], E2[NZ];
	  int size=sizeof(H2);
	  printf("\nProblem size= %d bytes\n\n",size*2);
	  
	  int k, TOTAL_TIME;
	 
	 

	  /* Initialize field components to zero*/
	  for ( k=0; k < NZ; k++ ) {
		H2[k] = 0;
		E2[k] = 0;		
	  }
	  
	  
	E2[NZ/2] = 1;
	  
	TOTAL_TIME = 100; //default 100
	printf("Max Tile Element size= %d bytes\n\n",(4*s*2*TOTAL_TIME));
	//skewed
	clock_t begin = clock();
	 	for (int ii = 0; ii < TOTAL_TIME + NZ; ii += s){
	 		
			for (int t = 0; t < TOTAL_TIME; t++) {
				
				for (int i = MAX(t, ii); i < MIN(t + NZ, ii + s); i++) {
				    H2[i - t] += 0.5*(E2[i - t+ 1] - E2[i - t]);        
				}
				for (int i = MAX(t +1, ii); i < MIN(t + NZ, ii + s); i++) {
				    E2[i - t] += 0.5*(H2[i - t] - H2[i - t -1 ]);
				    
				}
			}
		    
		}
	clock_t end = clock();
		/* At the end of the calculation, print out the Ex and Hy fields */
		//for ( k=0; k<= NZ; k++ )
		//  { printf( "%3d    %6.2f   %6.2f\n", k, E2[k], H2[k]); }

		  
		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d\n", NZ);
		printf( "Tile Width s = %d\n", s);

		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
		printf( "Time spent = %f\n", time_spent);
	}
	
