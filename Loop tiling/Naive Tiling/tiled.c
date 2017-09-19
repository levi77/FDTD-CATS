
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 16*1024*1024 
#define REP 1000
#define B  16*1024  
int main() { 
    int i,j,r;
    volatile char array[MAX];
    
   
    clock_t t1 = clock();
    
   

    for (i = 0; i < MAX; i += B) {
        for (r = 0; r < REP; r++) {
             for (j = i; j < (i + B); j+=64) {
                 array[j] = r;
             }
        }
    }
    clock_t t2 = clock();
    
    printf ("Tiled: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    
    printf ("array[0] = %d\n", array[0]);   
}

 //for (i = 0; i < MAX; i++) {  // warmup to make things equal if array happens to fit in your L3
      // array[i] = 0;
    //}
 // to prevent optimizing out all the writes last code part
