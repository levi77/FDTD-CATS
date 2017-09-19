//FDTD 2D NAIVE 

 //there is TM TE TEM mode
//EZ MODE 
   
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h> 
#include <time.h>  
//gcc -g -Wall FDTD.c -lm -o FDTD
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd ./FDTD
//cg_annotate outfdtd ~/Desktop/MASTER\'S/.c
#define IE 9  //for visual comparison test 60
#define JE 9 //for visual comparison test 60	
   
int main()   
{   
    float  ez[IE][JE], hx[IE][JE], hy[IE][JE];
    int size=sizeof(ez);
	printf("Problem size= %d bytes\n\n",size*3);   
    int n, i, j, TOTAL_TIME;   
    float T;   
    
    //for visual comparison on excel, uncomment
    FILE *fp;   
   
    
      
    
	for (j=0; j<JE; j++){      
        for (i=0;i<IE;i++){               
            ez[i][j]=0.0;   
            hx[i][j]=0.0;   
            hy[i][j]=0.0;               
        }           
    }   

    
    //t0=30;   
    //spread=6.0;   
    
    T=0;   
    TOTAL_TIME=10; //128  
   
      
        //printf("TOTAL_TIME --> ");    
clock_t begin = clock(); 
		       
        for (n=0; n<TOTAL_TIME; n++){   
            T=T+1;   
   
            /* start of the Main loop */   
   
            //calculate the Dz field   
            for(j=1;j<JE;j++){   
                for(i=1;i<IE;i++){   
                    ez[i][j]=ez[i][j]+0.5*(hy[i][j]-hy[i-1][j]-hx[i][j]+hx[i][j-1]);   
                }   
            }   
   
            //put a Guassian pulse in thye middle   
            //pulse=exp(-0.5*pow( (t0-T)/spread,2.0 ) );   
            //dz[IE/2][JE/2]=pulse;   
   			ez[IE/2][JE/2]=1;
   			
           
   
            //calculate the Hx field   
            for(j=0; j<=JE-1;j++){   
                for(i=0;i<IE-1;i++){   
                    hx[i][j]=hx[i][j]+0.5*(ez[i][j]-ez[i][j+1]);   
                }   
            }   
            //calculate the Hy field   
            for(j=0; j<JE-1;j++){   
                for(i=0;i<=IE-1;i++){   
                    hy[i][j]=hy[i][j]+0.5*(ez[i+1][j]-ez[i][j]);   
                }   
            }              
	}
	clock_t end = clock();
	/* write the E field to a file "Ez.xls" */   
 fp=fopen("Ez.dat", "w");   
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
