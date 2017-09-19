#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <time.h> 
//TEST FDTD time skewing
//L1 wall - 32768 bytes
int main() {
    
    std::vector<float> E1 = { 0, 0, 0, 0, 10, 0, 0, 0, 0 };
    std::vector<float> H1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    const int T = 100; //6 for very small test of above EH fields//number of time steps
    const int N = 10000000;//7 for very small test//FDTD 1D number of cells
    //const int s = 50000; //2 for very small test //tile width
	
        if (N > 7) {
            E1.resize(N + 1);
            H1.resize(N + 1);
            
         }
     printf( "Problem space NZ = %d ",N);
    int count = 0;
    int size= (sizeof(std::vector<float>) + (sizeof(float) * E1.size()))*2;
    std::cout << "size of arrays= " << size*2 << '\n';
    //simple FDTD calculations
    clock_t begin1 = clock();     //initializing of the clock with the shortest tick period available 
  
                     //returns a std::chrono::time_point representing the current value of the clock
    
    for (int t = 0; t < T; ++t) { //time loop
        for (int i = 0; i < N; ++i) //H field calculations: H field indices scope is 0 to N-1, for E field from 0 to N
        H1[i] += 0.5f*(E1[i+1] - E1[i]);
        for (int i = 0; i < N-1; ++i) //attention N-1! //E field calculations: E field indices scope is 0 to N-1, for H from 0 to N-1
        E1[i+1] += 0.5f*(H1[i+1] - H1[i]);
        //nodes E at border E1[0]=E1[N]=0 are not updated
        //        E1[0] += 0.5*(H1[0]);
        //        E1[4] = 10;
        //            count++;
    }
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC; 
		printf( "Naive time = %f\n", time_spent1); 
   std::cout << "count= " << count << '\n';
}

