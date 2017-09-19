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
#define nx  1000  //for visual comparison test 60
#define ny  1000 //for visual comparison test 60	
#define MAX(a,b) ( ( a > b) ? a : b ) 
#define MIN(a,b) ( ( a < b) ? a : b ) 
int main()   
{   
	const int TOTAL_TIME=100;//100;

    const int s = 20; 
	printf("\n Tile Width: %d  ",s);
    //float  hz[nx][ny], ex[nx][ny+1], ey[nx+1][ny];
    //float  hz2[nx][ny], ex2[nx][ny+1], ey2[nx+1][ny];
	std::vector<std::vector<float>> hz(nx), ex(nx), ey(nx + 1);
	std::vector<std::vector<float>> hz2(nx), ex2(nx), ey2(nx + 1);
	for (int i = 0;i < nx;i++) {
		hz[i].resize(ny);
		ex[i].resize(ny+1);
		ey[i].resize(ny);
		hz2[i].resize(ny);
		ex2[i].resize(ny + 1);
		ey2[i].resize(ny);
	}
	ey[nx].resize(ny);
	ey2[nx].resize(ny);

	int size=sizeof(hz[0][0])*nx*ny;
	printf("Problem size= %d bytes\n\n",size*3);   
    int t, i,j;   
    float T;      
    
    //for visual comparison on excel, uncomment
    FILE *fp;   
   
    
   for (i=0;i<nx;i++){
		for (j=0; j<ny; j++){ 
            hz[i][j]=0.0;
            hz2[i][j]=0.0;                 
        }           
    }
	for (i=0;i<nx;i++){
		for (j=0; j<ny+1; j++){  
            ex[i][j]=0.0;
            ex2[i][j]=0.0;                
        }           
    }  
	for (i=0;i<nx+1;i++){ 
		for (j=0; j<ny; j++){ 
            ey[i][j]=0.0;
            ey2[i][j]=0.0;                 
        }           
    }
   
	 
    
      
     
    printf("Max Tile Element size= %d bytes\n\n",(8*s*TOTAL_TIME));
    
	//hz[nx / 2][ny / 2] = 1;

    //naive code
    hz[nx / 2][ny / 2] = 1;
    clock_t begin1 = clock(); 

        for(t=0; t<TOTAL_TIME; t++)  {
		for (j=0; j<ny; j++){
			for (i=1; i<nx; i++){
				   ey[i][j] = ey[i][j] - 0.5*(hz[i][j]-hz[i-1][j]);
				    }
			    }
			
		for (j=1; j<ny; j++){
			for (i=0; i<nx; i++){
				    ex[i][j] = ex[i][j] - 0.5*(hz[i][j]-hz[i][j-1]);
			    }
		    }
			
		for (j=0; j<ny; j++){
			for (i=0; i<nx; i++){
				    hz[i][j]=hz[i][j]-0.5*(ex[i][j+1]-ex[i][j]+ey[i+1][j]-ey[i][j]);
			    }
		    }
			    //hz[nx / 2][ny / 2] = 1;
    }
    clock_t end1 = clock();
    
	//write_EM_field_array_to_file_hz(hz, "hz_naive.dat");
	//write_EM_field_array_to_file_ex(ex, "ex_naive.dat");
	//write_EM_field_array_to_file_ey(ey, "ey_naive.dat");

	//skewed code
	clock_t begin2 = clock();              

	//hz2[nx / 2][ny / 2] = 1;
	hz2[nx / 2][ny / 2] = 1;
	for (int ii = 0; ii < TOTAL_TIME + ny; ii += s){	 

		for(t=0; t<TOTAL_TIME; t++){

			for (int j = MAX(t, ii); j < MIN(t+ny, ii + s); j++) { //time-skew
				for (i=1; i<nx; i++){ 	//iterating over X field as CATS-1 paper says.
					ey2[i][j-t] = ey2[i][j-t] - 0.5*(hz2[i][j-t]-hz2[i-1][j-t]);
				}
				
			}
			
			for (int j = MAX(t+1, ii); j < MIN(t +ny, ii + s); j++) {
				for (i=0; i<nx; i++){ 	 	
				     ex2[i][j-t] = ex2[i][j-t] - 0.5*(hz2[i][j-t]-hz2[i][j-1-t]);
			    }
	    	}
   
		
			for (int j = MAX(t, ii-1); j < MIN(t +ny, ii-1 + s); j++) { 	
				for (i=0; i<nx; i++){
					hz2[i][j-t] = hz2[i][j-t] - 0.5*(ex2[i][j + 1-t] - ex2[i][j-t] + ey2[i + 1][j-t] - ey2[i][j-t]);
		    		//hz2[i-t][j]= i - t == nx / 2 && j == ny / 2 ? 1 : hz2[i-t][j]-0.5*(ex2[i-t][j+1]-ex2[i-t][j]+ey2[i+1-t][j]-ey2[i-t][j]);
				}
			}
			
		}	
	} 
	clock_t end2 = clock();
//VISUALIZATION BY PRINTING OUT hz FIELD (COMMENT ON VALGRIND RUNS)
	//write_EM_field_array_to_file_hz(hz2, "hz_skewed.dat");
	//write_EM_field_array_to_file_ex(ex2, "ex_skewed.dat");
	//write_EM_field_array_to_file_ey(ey2, "ey_skewed.dat");

	fp=fopen("hz_naive_vs_time-block.dat", "w");   
   
    for(i=0;i<nx;i++){   
		for(int j=0;j<ny;j++){  
            fprintf(fp, "%6.3f \t",hz[i][j]);   
        }   
        fprintf(fp,"\n");   
    }
    fprintf(fp,"\n\n");
    
	  
	for(i=0;i<nx;i++){ 
		for(int j=0;j<ny;j++){   
	 	  fprintf(fp, "%6.3f \t",hz2[i][j]);   
	}   
	fprintf(fp,"\n");   
	}
	fclose(fp);     
 	
 	
	
//	if (memcmp(hz, hz2, sizeof(hz)) == 0)
    if (hz==hz2 && ex==ex2 && ey==ey2)
	 printf("Vectors H1 i E1 are equal to H2 E2 - simple algorithm and time skewing have the same results\n");
    else
    	printf("simple algorithm and time skewing have different results\n");

		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d x %d\n",nx,ny);
	  		
		double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC; 
		double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
		printf( "Naive time = %f\n", time_spent1); 
		printf( "Skewed time = %f\n", time_spent2);     
   
}
