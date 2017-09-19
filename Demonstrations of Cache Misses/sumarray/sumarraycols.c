#include <stdio.h>
#include <stdlib.h>

#define M 5000
#define N M

int a[M][N];

/* $begin sumarraycols */
int sumarraycols(int a[M][N]) 
{  	
    int i, j, sum = 0;

    for (j = 0; j < N; j++)
		for (i = 0; i < M; i++) 
			sum += a[i][j];
    return sum;	
}
/* $end sumarraycols */


int main()
{

    int i, j, size;
	size=sizeof(a);
	printf("%d\n",size);
    for (i = 0; i < M; i++) 
		for (j = 0; j < N; j++)
			a[i][j] = ((j%2 == 0) ? 1 : -1);

    /* should sum to zero */
    printf("sum=%d\n", sumarraycols(a));
    exit(0);
}

//gcc -g sumarrayrows.c -o sumarrayrows && ./sumarrayrows  && valgrind --tool=cachegrind --cachegrind-out-file=outrows ./sumarrayrows
