//Tiling

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 16*1024*1024 
#define REP 1000

int main() { 
    int i,r;
    volatile char array[MAX];
	

    clock_t t1 = clock();
    
    // un-tiled loop
    for (r = 0; r < REP; r++) {
        for (i = 0; i < MAX; i+=64) {	
             array[i]=r; 
                 
        }
    }
    
    clock_t t2 = clock();
    
    printf ("Untiled: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    
    printf ("array[0] = %d\n", array[0]);   
}


//volatile char array2[MAX];
    // warmup to make things equal if array happens to fit in your L3
    // this line places some of the data to cache, when you remove this line we observe %100 write misses
    
    
    //TODO: TRY WITH DIFFERENT DATA TYPES.
    //for (i = 0; i < MAX; i++) {  
    //  array[i] = 0;
      //array2[i] = 0;
    //} //array[i]=array2[i];
             //array[i]+=array2[i];
             //array[i]+=0.5*(array2[i+1]-array2[i]);
             //array[i] = array[i+1]; //transfer write misses to read
             //array[i+1] =r; //and MAX-1 half the write misses        
