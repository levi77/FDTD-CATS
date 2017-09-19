//fd2d_3.1.c   2D TM program   
 //there is TM TE TEM mode
//hz MODE 
#define _CRT_SECURE_NO_WARNINGS   

#include <math.h>   
#include <stdio.h>   
#include <stdlib.h> 
#include <time.h> 

# include <string.h> 
#include <vector>
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c
//#define nx 21 // 512  //for visual comparison test 60
//#define ny 21 //512 //for visual comparison test 60	
#define nx  1100 //for visual comparison test 60
#define ny  1100 //for visual comparison test 60	
#define MAX(a,b) ( ( a > b) ? a : b ) 
#define MIN(a,b) ( ( a < b) ? a : b ) 
int main()   
{   
	const int TOTAL_TIME=100;//100;
	printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d x %d\n",nx,ny);
    //float  hz[nx][ny], ex[nx][ny+1], ey[nx+1][ny];
    //float  hz2[nx][ny], ex2[nx][ny+1], ey2[nx+1][ny];
	std::vector<std::vector<float>> hz(nx), ex(nx), ey(nx + 1);
	for (int i = 0;i < nx;i++) {
		hz[i].resize(ny);
		ex[i].resize(ny+1);
		ey[i].resize(ny);
	}
	ey[nx].resize(ny);

	
    int t, i,j;   
    float T;      
    
    //for visual comparison on excel, uncomment
    FILE *fp;   
   
    
   for (i=0;i<nx;i++){
		for (j=0; j<ny; j++){ 
            hz[i][j]=0.0;
                
        }           
    }
	for (i=0;i<nx;i++){
		for (j=0; j<ny+1; j++){  
            ex[i][j]=0.0;
             
        }           
    }  
	for (i=0;i<nx+1;i++){ 
		for (j=0; j<ny; j++){ 
            ey[i][j]=0.0;
               
        }           
    }
    int size=sizeof(hz[0][0])*nx*ny;
    printf("One vector size= %d bytes\n\n",size);   
	printf("Total problem size= %d bytes\n\n",size*3);   
	
	hz[nx / 2][ny / 2] = 1;

    //naive code

    clock_t begin1 = clock(); 

         for(t=0; t<TOTAL_TIME; t++)  {

			for (i=1; i<nx; i++){
				for (j=0; j<ny; j++){
				   ey[i][j] = ey[i][j] - 0.5*(hz[i][j]-hz[i-1][j]);
				    }
			    }
			for (i=0; i<nx; i++){
				for (j=1; j<ny; j++){
				    ex[i][j] = ex[i][j] - 0.5*(hz[i][j]-hz[i][j-1]);
			    }
		    }
			for (i=0; i<nx; i++){
				for (j=0; j<ny; j++){
				    hz[i][j]=hz[i][j]-0.5*(ex[i][j+1]-ex[i][j]+ey[i+1][j]-ey[i][j]);
			    }
		    }
			    //hz[nx / 2][ny / 2] = 1;
    }
    clock_t end1 = clock();


	fp=fopen("hz_naive.dat", "w");   
   
    for(i=0;i<nx;i++){   
		for(int j=0;j<ny;j++){  
            fprintf(fp, "%6.3f \t",hz[i][j]);   
        }   
        fprintf(fp,"\n");   
    }

	fclose(fp);     
		double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC; 
		printf( "Naive time = %f\n", time_spent1); 
   
   
}


