/* FD1D_1.1.c.   1D FDTD simulation in free space
with absorbing conditions */

//COMPARISON MAY GIVE DIFFERENT RESULTS BUT BECUASE OF MINUS STUFF
//NEED TO COMPARE ABSOLUTE VALUES.

//CMD EVALUATIONS
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c

//HARD-CODED TIME STEP.

// L1 Threshold NZ: 4096 
// L3 Threshold NZ: 6291456 

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <string.h>

#define NZ  10000000    /* NZ is the number of cells to be used */
//experimenting with the step size to see the changes at the cache performance

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


int main ()
{
    
      const int s = 50;//(4096); //2 for very small test //tile width 50000
	  
	  float H1[NZ], E1[NZ]; //field components
	  float H2[NZ], E2[NZ];
	  int size=sizeof(H2);
	  printf("Problem size= %d bytes\n\n",size*2);
	  float T;
	  int k, TOTAL_TIME;
	 

	  /* Initialize field components to zero*/
	  for ( k=0; k < NZ+1; k++ ) {
		H1[k] = 0;
		E1[k] = 0;
		H2[k] = 0;
		E2[k] = 0;		
	  }
	  
	  //PUT EXCITATION HERE
	  
	  E1[NZ/2] = 1;
	  E2[NZ/2] = 1;
	  T=0;	 
      TOTAL_TIME = 100;
      printf("Max Tile Element size= %d bytes\n\n",(4*s*TOTAL_TIME));
	  	
	 clock_t begin1 = clock(); 
	//unskewed
		for (int t = 0; t < TOTAL_TIME; t++) { 
			T=T+1;
			for (int i = 0; i < NZ; ++i) {
				H1[i] += 0.5f*(E1[i+1] - E1[i]);
			}				
			for ( int i = 1; i < NZ; ++i ) {
				E1[i] += 0.5f*(H1[i] - H1[i-1]);
			} 
			//other version was i=0, NZ-1 with E1[i+1]+=0.5f*(H1[i+1] - H1[i]);
   		}
   	clock_t end1 = clock();	
   	
		//for ( k=0; k<= NZ; k++ )
		//{ printf( "%3d    %6.2f   %6.2f\n", k, E1[k], H1[k]); }
	//printf("END OF THE UNSKEWED CODE\n\n\n\n\n\n");
	clock_t begin2 = clock();
	//skewed
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
	clock_t end2 = clock();	
		//for ( k=0; k<= NZ; k++ )
		//{ printf( "%3d    %6.2f   %6.2f\n", k, E2[k], H2[k]); }
	
//++++++COMPARISON OF RESULTS++++++++//	
	for ( k=0; k < NZ+1; k++ ) {
		H1[k] = abs(H1[k]);
		E1[k] = abs(E1[k]);
		H2[k] = abs(H2[k]);
		E2[k] = abs(E2[k]);		
	  }

	if (memcmp(E1, E2, sizeof(E1)) == 0 && memcmp(H1, H2, sizeof(H1)) == 0)
    	printf("Vectors H1 i E1 are equal to H2 E2 - simple algorithm and time skewing have the same results\n");
    else
    	printf("simple algorithm and time skewing have different results\n");
		
		//printf( "Total time T = %5.0f\n", T);
		printf( "Problem space NZ = %d\n", NZ);
	  		
		
		double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC; 
		double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC; 
		printf( "Naive Time = %f\n", time_spent1);
		printf( "Skewed Time = %f\n", time_spent2);
	}
	
