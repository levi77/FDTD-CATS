//Tiling
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd2 ./FDTD2
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//valgrind --tool=cachegrind --main-stacksize=33554432 --cachegrind-out-file=outuntiled ./untiled
#define MAX 100*1024 //16MB - L3 size is 6144 KB
#define REP 1000
#define B  4*1024    //16 KB L1 size blocking

//AS WE ARE WORKING WITH FLOATS (4BYTES) WE DIVIDE ALL THE MAX AND B SIZES BY 4

int main() { 
    int i,j,r;
    volatile float array[MAX];
    
    //for (i = 0; i < MAX; i++) {  // warmup to make things equal if array happens to fit in your L3
      // array[i] = 0;
    //}

    
    
   for(int z=0;z<MAX;z+=2048){
		clock_t t1 = clock();
		// Tiled loop
		for (i = 0; i < z; i += B) {
		    for (r = 0; r < REP; r++) {
		         for (j = i; j < (i + B); j+=64) {
		             array[j] = r;
		         }
		    }
		}
		clock_t t2 = clock();
    printf ("NZ = %d Tiled: %f sec\n", z, (double)(t2 - t1) / CLOCKS_PER_SEC);
    }
    
    
    
    printf ("array[0] = %f\n", array[0]);    // to prevent optimizing out all the writes
}


