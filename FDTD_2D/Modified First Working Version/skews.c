//fd2d_3.1.c   2D TM program   
 //there is TM TE TEM mode
//EZ MODE 
   
//THIS ONE IS SKEWED ONLY AT Y INDICES   
   
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h> 
#include <time.h>  
#include <string.h>
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c
#define IE 7 //for visual comparison test 60
#define JE 7 //for visual comparison test 60	

#define MAX(a,b) ( ( a > b) ? a : b ) 
#define MIN(a,b) ( ( a < b) ? a : b ) 

int main()   
{   
    const int s = 2; 
	printf("\n Tile Width: %d",s);
    
    
    float  ez1[IE][JE], hx1[IE][JE], hy1[IE][JE];
    float  ez[IE][JE], hx[IE][JE], hy[IE][JE];
    int size=sizeof(ez);
	printf("Problem size= %d bytes\n\n",size*3);   
    int n, i,j,TOTAL_TIME;   
    float T;      
    
    //for visual comparison on excel, uncomment
    FILE *fp;   
   
    
      
   
    for (int j=0; j<JE; j++){      
        for (i=0;i<IE;i++){               
            ez[i][j]=0.0;   
            hx[i][j]=0.0;   
            hy[i][j]=0.0; 
            ez1[i][j]=0.0;   
            hx1[i][j]=0.0;   
            hy1[i][j]=0.0;                  
        }           
    }   
    
    T=0;   
    TOTAL_TIME=4; //128    
    printf("Max Tile Element size= %d bytes\n\n",(4*s*TOTAL_TIME));
     clock_t begin1 = clock();
    //NAIVE
      for (n=0; n<TOTAL_TIME; n++){   
            T=T+1;   
   
            /* start of the Main loop */   
   
            //calculate the Dz field   
            for(j=1;j<JE;j++){   
                for(i=1;i<IE;i++){   
                    ez1[i][j]=ez1[i][j]+0.5*(hy1[i][j]-hy1[i-1][j]-hx1[i][j]+hx1[i][j-1]);   
                }   
            }   
   
            //put a Guassian pulse in thye middle   
            //pulse=exp(-0.5*pow( (t0-T)/spread,2.0 ) );   
            //dz[IE/2][JE/2]=pulse;   
   			ez1[IE/2][JE/2]=1;
   			
           
   
            //calculate the Hx field   
            for(j=0; j<JE-1;j++){   
                for(i=0;i<IE;i++){   
                    hx1[i][j]=hx1[i][j]+0.5*(ez1[i][j]-ez1[i][j+1]);   
                }   
            }   
            //calculate the Hy field   
            for(j=0; j<JE;j++){   
                for(i=0;i<IE-1;i++){   
                    hy1[i][j]=hy1[i][j]+0.5*(ez1[i+1][j]-ez1[i][j]);   
                }   
            }              
	}
    clock_t end1 = clock();
    
    //ONLY CHANGE IS MAX(n+1) is changed at ez outer loop.
    //SKEWED
	clock_t begin = clock();              
    for (int ii = 0; ii < TOTAL_TIME + JE; ii += s){    
        for (n=0; n<TOTAL_TIME; n++){   
            T=T+1;   
   
            /* start of the Main loop */   
   
            //calculate the Dz field   
             
            for(i=1;i<IE;i++){   //changed max (n+1) from (n)
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

    fp=fopen("Ez_comparison.dat", "w");   
    for(int j=0;j<JE;j++){   
        for(i=0;i<IE;i++){   
            fprintf(fp, "%6.3f \t",ez1[i][j]);   
        }   
        fprintf(fp,"\n");   
    } 
    fprintf(fp,"\n\n\n");  
     for(int j=0;j<JE;j++){   
        for(i=0;i<IE;i++){   
            fprintf(fp, "%6.3f \t",ez[i][j]);   
        }   
        fprintf(fp,"\n");   
    }     
    fclose(fp);   
    
    
	if (memcmp(ez1, ez, sizeof(ez)) == 0)
    	printf("Vectors H1 i E1 are equal to H2 E2 - simple algorithm and time skewing have the same results\n");
    else
    	printf("simple algorithm and time skewing have different results\n");


		printf( "Total time T = %d\n", TOTAL_TIME);
		printf( "Problem space NZ = %d x %d\n",IE,JE);
	  		
		double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;  
		printf( "NAIVE TIME = %f\n", time_spent1);
		printf( "SKEWED TIME = %f\n", time_spent);    
   
}
