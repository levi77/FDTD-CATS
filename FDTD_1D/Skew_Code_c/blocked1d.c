/* FD1D_1.1.c.   1D FDTD simulation in free space
with absorbing conditions */

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
//L3 NZ = 800000

#define NZ  100000     /* NZ is the number of cells to be used */
#define B 4000
//E1perimenting with the step size to see the changes at the cache performance
int main ()
{
	 //6 for very small test of above EH fields//number of time steps
    
    //const int s = 1; //2 for very small test //tile width 50000
	   
	  float E1[NZ], H1[NZ];//field components
	  int size=sizeof(H1);
	  printf("Problem size= %d bytes\n\n",size*2);
	  printf("One array size= %d bytes\n",size);
	  int k, TOTAL_TIME;
	  float T;
	  printf("Block size= %d bytes\n\n", B*4);

	  /* Initialize field components to zero*/
	  for ( k=0; k < NZ+1; k++ ) {
		H1[k] = 0;
		E1[k] = 0;
			
	  }
	  
	  
	  H1[NZ/2] = 1;
	  T=0;
	  
	TOTAL_TIME = 100;
	  	//default 100
	clock_t begin = clock();
	//naive
	for (int z = 0; z < NZ; z += B) {
		for (int t = 0; t < TOTAL_TIME; t++) { 
			T=T+1;
				for (int i = z; i < (z+B); i++) {
		      H1[i] += 0.5f*(E1[i+1] - E1[i]);
		    }
		if(z!=NZ-B){	
			for ( int i = z; i < (z+B); i++ ) {
		      E1[i+1] += 0.5f*(H1[i+1] - H1[i]);
		    } 
	    }else{
	    	for ( int i = z; i < (z+B-1); i++ ) {
		      E1[i+1] += 0.5f*(H1[i+1] - H1[i]);
		    }
	    	} 
		}
	}
		/* End of the Main FDTD Loop */

		/* At the end of the calculation, print out the E1 and H1 fields */
		  //for ( k=0; k<= NZ; k++ )
		  //{ printf( "%3d    %6.2f   %6.2f\n", k, E1[k], H1[k]); }

		  
		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d\n", NZ);
  		printf( "Block size B = %d\n", B);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
		printf( "Time spent = %f\n", time_spent);
	}
	
