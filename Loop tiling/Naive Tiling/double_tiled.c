//Double Tiling
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd2 ./FDTD2
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 16*1024*1024 //16MB - L3 size is 6144 KB
#define REP 1000
#define B3  6144*1024
#define B1  32*1024    //32 KB L1 size blocking

int main() { 
    int i,j,r,z;
    char array[MAX];
    
    for (i = 0; i < MAX; i++) {  // warmup to make things equal if array happens to fit in your L3
       array[i] = 0;
    }

    clock_t t1 = clock();
    
   
    //Double Tiled loop
    for(z=0; z<MAX; z+=B3){
	    for (i = z; i < (z+B3); i += B1) {
		for (r = 0; r < REP; r++) {
		     for (j = i; j < (i + B1); j+=64) {
		         array[j] = r;
		     }
		}
	    }
    }
    clock_t t2 = clock();
    
    printf ("Double Tiled: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    
    printf ("array[0] = %d\n", array[0]);    // to prevent optimizing out all the writes
}


