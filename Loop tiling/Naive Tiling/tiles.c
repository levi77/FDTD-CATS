//http://stackoverflow.com/questions/20367246/loop-tiling-how-to-choose-block-size/20372396#20372396

//Problem of segmentation fault: http://stackoverflow.com/questions/4220965/segmentation-fault-due-to-lack-of-memory-in-c

//cg_annotate icin gcc -g ile basla compile

//ulimit -s 32767 - This command run in CMD before program starts to change default program stack size to avoid stack overflow (or unlimited instead 32kb)

//valgrind --tool=cachegrind --main-stacksize=33554432 --cachegrind-out-file=outuntiled ./untiled --valgrind command to avoid stack overflow


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//for L3 hit version, use MAX 6144*1024
#define MAX 16*1024*1024 //16MB - L3 size is 6144 KB
#define REP 1000
#define B  32*1024    //32 KB L1 size blocking

//Both loops are doing the same accesses (the 64byte jumps is to stress the caches by using each cache line once, and preventing the IPC and instruction dispatching from being the bottleneck).

int main() { 
    int i,j,r;
    char array[MAX]; //char = 1 byte

    for (i = 0; i < MAX; i++) {  // warmup to make things equal if array happens to fit in your L3
       array[i] = 0;
    }

    clock_t t1 = clock();

    // Tiled loop
    for (i = 0; i < MAX; i += B) {
        for (r = 0; r < REP; r++) {
             for (j = i; j < (i + B); j+=64) {
                 array[j] = r;
             }
        }
    }
    clock_t t2 = clock();

    // un-tiled loop
    for (r = 0; r < REP; r++) {
        for (i = 0; i < MAX; i+=64) {
             array[i] = r;
        }
    }

    clock_t t3 = clock();
    printf ("Tiled: %f sec\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf ("Untiled: %f sec\n", (double)(t3 - t2) / CLOCKS_PER_SEC);
    printf ("array[0] = %d\n", array[0]);    // to prevent optimizing out all the writes
}

//REMARKS

//We can see the rise of LL misses by increasing MAX from 6144KB to 8000KB (%100)

//Untiled code has %20 more misses than tiled code (running and analysing individual codes

//Tiled code runs faster in this test

//Compiler optimizations make code much faster but Tiled code is still much faster
