//Tiling

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//for L3 hit version, use MAX 6144*1024
#define MAX 60000 //16MB - L3 size is 6144 KB
#define REP 10000
//valgrind --tool=cachegrind --main-stacksize=33554432 --cachegrind-out-file=outuntiled ./untiled
int main() { 
    int i,r;
    volatile float array[MAX];
	//volatile char array2[MAX];
    // warmup to make things equal if array happens to fit in your L3
    // this line places some of the data to cache, when you remove this line we observe %100 write misses
    
   // for (i = 0; i < MAX; i++) {  
    //  array[i] = 0;
      //array2[i] = 0;
    //}

    for(int z=0;z<MAX;z+=200){
    clock_t t1 = clock();
    // un-tiled loop
    for (r = 0; r < REP; r++) {
        for (i = 0; i < z; i+=64) {	//Iterating 64 bytes to fulfill cacheline
             array[i]=r; //main test
             //array[i]=array2[i];
             //array[i]+=array2[i];
             //array[i]+=0.5*(array2[i+1]-array2[i]);
             //array[i] = array[i+1]; //transfer write misses to read
             //array[i+1] =r; //and MAX-1 half the write misses             
        }
    }
    clock_t t2 = clock();
    
    printf ("Untiled: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    //printf ("array[0] = %d\n", array[0]);    // to prevent optimizing out all the writes
   }
    
    
    
}
