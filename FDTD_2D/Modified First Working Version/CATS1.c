//fd2d_3.1.c   2D TM program   
 //there is TM TE TEM mode
//EZ MODE 
   
//THIS ONE IS SKEWED ONLY AT Y INDICES   
   
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h> 
#include <time.h>  
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c
#define IE 1000  //for visual comparison test 60
#define JE 1000 //for visual comparison test 60	

#define MAX(a,b) ( ( a > b) ? a : b ) 
#define MIN(a,b) ( ( a < b) ? a : b ) 

int main()   
{   
    const int s = 40; 
	printf("\n Tile Width: %d",s);
    
    
    float  ez[IE][JE], hx[IE][JE], hy[IE][JE];
    int size=sizeof(ez);
	printf("Problem size= %d bytes\n\n",size*3);   
    int n, i, TOTAL_TIME;   
    float T;      
    
    //for visual comparison on excel, uncomment
    FILE *fp;   
   
    
      
   
    for (int j=0; j<JE; j++){      
        for (i=0;i<IE;i++){               
            ez[i][j]=0.0;   
            hx[i][j]=0.0;   
            hy[i][j]=0.0;               
        }           
    }   
    
    T=0;   
    TOTAL_TIME=100; //128    
    printf("Max Tile Element size= %d bytes\n\n",(4*s*TOTAL_TIME));
    
	clock_t begin = clock();              
    for (int ii = 0; ii < TOTAL_TIME + JE; ii += s){    
        for (n=0; n<TOTAL_TIME; n++){   
            T=T+1;   
   
            /* start of the Main loop */   
   
            //calculate the Dz field   
             
            for(i=1;i<IE;i++){   
        		for (int j = MAX(n+1, ii); j < MIN(n + JE, ii + s); j++) {  
                    ez[i][j-n]=ez[i][j-n]+0.5*(hy[i][j-n]-hy[i-1][j-n]-hx[i][j-n]+hx[i][j-n-1]);   
                }   
            }   
   
            //put a Guassian pulse in thye middle   
            //pulse=exp(-0.5*pow( (t0-T)/spread,2.0 ) );   
            //dz[IE/2][JE/2]=pulse;   
   			ez[IE/2][JE/2]=1;
   			
           
   
            //calculate the Hx field   
            for(i=0;i<IE;i++){  
		        for (int j = MAX(n, ii)-1; j < MIN((n + JE), ii + s)-1; j++) {        
		                hx[i][j-n]=hx[i][j-n]+0.5*(ez[i][j-n]-ez[i][j-n+1]);   
		            }   
            }   
            //calculate the Hy field 
            for(i=0;i<IE-1;i++){  
            	for (int j = MAX(n, ii); j < MIN(n + JE, ii + s); j++) {				       
				        hy[i][j-n]=hy[i][j-n]+0.5*(ez[i+1][j-n]-ez[i][j-n]);   
				    }   
				} 
			}  
}   
	clock_t end = clock();
//VISUALIZATION BY PRINTING OUT EZ FIELD (COMMENT ON VALGRIND RUNS)

    fp=fopen("CATS1.dat", "w");   
    for(int j=0;j<JE;j++){   
        for(i=0;i<IE;i++){   
            fprintf(fp, "%6.3f \t",ez[i][j]);   
        }   
        fprintf(fp,"\n");   
    }   
    fclose(fp);   
  
	

		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d x %d\n",IE,JE);
	  		
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
		printf( "Time spent = %f\n", time_spent);   
   
}
