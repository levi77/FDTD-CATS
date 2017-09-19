/* FD1D_1.1.c. VERSION 2  1D FDTD simulation in free space
with absorbing conditions */

//THIS CODE COULD BE BETTER WITH A FOR LOOP, THEN ADDING EVERY TIME RESULTS TO EACH OTHER IN THE END INSTEAD EVALUATING EVERYTIME A NEW ONE

//CMD EVALUATIONS
//gcc -g -Wall FDTD2.c -lm -o FDTD2
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd2 ./FDTD2
//cg_annotate outfdtd2 ~/Desktop/MASTER\'S/FDTD2.c

//HARD-CODED TIME STEP.

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
//first HH value was 500000 
#define HH  1000000     /* NZ is the number of cells to be used */
//experimenting with the step size to see the changes at the cache performance
#define BILLION 1E9

int main ()
{
	FILE *fp, *fopen();
	fp = fopen( "time3.xls", "w");
	struct timespec requestStart, requestEnd;
	for(int NZ=500000;NZ<=HH;NZ+=2000){//first NZ value was 200,increment now= 2000
	  fprintf(fp, "%d\t", NZ);
	  clock_gettime(CLOCK_MONOTONIC, &requestStart);
	  
	  float ex[NZ], hy[NZ]; //field components
	  float ex_low_m1,ex_low_m2,ex_high_m1,ex_high_m2;
	  int k, kc, TOTAL_TIME;
	  float T;
	  float t0, spread, pulse;
	 

	  /* Initialize field components to zero*/
	  for ( k=0; k < NZ+1; k++ ) {
		ex[k] = 0;
		hy[k] = 0;
		
	  }
	  
	  kc = NZ/2;    /* Center of the problem space */
	  t0 = 40.0;    /* Center of the incident pulse */
	  spread = 10;  /* Width of the incident pulse */
	  T = 0;		//current time when STEPS amount iterated.
	 

	  	TOTAL_TIME=1000;
	  
	
		for (int n = 0; n <= (TOTAL_TIME-1); n++) {
		  T = T + 1;        /* T keeps track of the total number */
		                    /* of times the main loop is executed */
		  /* Main FDTD Loop */
		
		
		    /* Calculate the Ex field */
		    
		    for ( k = 1; k < NZ; k++ ) {
		      ex[k] = ex[k] + 0.5*(hy[k-1] - hy[k]);
		    }
		    
		
		    /* Put a Gaussian pulse in the middle */
		    pulse = exp(-(pow( (t0-T)/spread, 2.0 )));
		    
		    ex[kc] = pulse;
		    //printf("pulse at center: T = %5.0f  (ex):%6.2f (hy):%6.2f\n", T, ex[kc],hy[kc]);

			//Absorbing Boundary Conditions
			ex[0]= ex_low_m2;
			ex_low_m2=ex_low_m1;
			ex_low_m1=ex[1];	
			
		
			ex[NZ-1]=ex_high_m2;
			ex_high_m2=ex_high_m1;
			ex_high_m1=ex[NZ-2];
		
		    /* Calculate the Hy field */
		    for ( k = 0; k < NZ-1; k++ ) {
		      hy[k] = hy[k] + 0.5*(ex[k] - ex[k+1] );
		    }
		    
		    
		}

		/* End of the Main FDTD Loop */

		/* At the end of the calculation, print out the Ex and Hy fields */
		 // for ( k=0; k<= NZ; k++ )
		  //{ printf( "%3d    %6.2f   %6.2f\n", k, ex[k], hy[k]); }

		  
		//printf( "\n\nTotal time T = %5.0f\n\n", T);
		//printf( "Problem space NZ = %d\n\n", NZ);
	  		
		clock_gettime(CLOCK_MONOTONIC, &requestEnd);
	
		double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
  + ( requestEnd.tv_nsec - requestStart.tv_nsec )
  / BILLION;
		//printf( "Time spent = %f\n\n", time_spent);
		
		fprintf(fp, "%f\n", accum);
	}
	//close Excel
	fclose(fp);
}
