//FDTD1D.C

//MODE EX/HY

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


#define NZ  100000      /* NZ is the number of cells to be used */
//E1perimenting with the step size to see the changes at the cache performance
int main ()
{
	 //6 for very small test of above EH fields//number of time steps
    
    //const int s = 1; //2 for very small test //tile width 50000
	   
	  float ex[NZ], hy[NZ]; //field components
	  int size=sizeof(hy);
	  printf("Problem size= %d bytes\n\n",size*2);
	  printf("One array size= %d bytes\n",size);
	  int k, TOTAL_TIME;
	  float T;

	  

	  /* Initialize field components to zero*/
	  for ( k=0; k < NZ+1; k++ ) {
		hy[k] = 0;
		ex[k] = 0;
			
	  }
	  
	  
	  
	  T=0;
	ex[NZ/2] = 1;  
	TOTAL_TIME = 100;
	  	//default 100
	clock_t begin = clock();
	//unskewed
		for (int t = 0; t < TOTAL_TIME; t++) { 
			T=T+1;
				for (int i = 0; i < NZ; i++) {
		      hy[i] += 0.5f*(ex[i+1] - ex[i]);
		    }
		    //pulse = exp(-(pow( (t0-T)/spread, 2.0 )));
			   	//hy[kc] = pulse;
		    
			for ( int i = 1; i < NZ; ++i ) {
				ex[i] += 0.5f*(hy[i] - hy[i-1]);
			} 
    }
	clock_t end = clock();
		/* End of the Main FDTD Loop */

		/* At the end of the calculation, print out the ex and hy fields */
		  //for ( k=0; k<= NZ; k++ )
		  //{ printf( "%3d    %6.2f   %6.2f\n", k, ex[k], hy[k]); }

		  
		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d\n", NZ);
	  		
		
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
		printf( "Time spent = %f\n", time_spent);
	}
	
