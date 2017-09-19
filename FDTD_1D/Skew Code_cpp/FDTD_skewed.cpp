#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
//g++ -g -Wall -std=c++11  FDTD_skewed.cpp -o FDTD_skewed
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd_skewed ./FDTD_skewed
//TEST FDTD time skewing
int main() {
    
    std::vector<float> E2 = { 0, 0, 0, 0, 10, 0, 0, 0, 0 };
    std::vector<float> H2 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//normal values: T=100 , N=1000000, s=50000
    const int T = 100; //6 for very small test of above EH fields//number of time steps
    const int N = 1000000;//7 for very small test//FDTD 1D number of cells
    const int s = 40; //2 for very small test //tile width 50000
    std::cout << "T= " << T << '\n';
    std::cout << "N= " << N << '\n';
    std::cout << "Tile width= " << s << '\n';
    std::cout << "Max tile size= " << s*4*100 << '\n';
    //for s=10000, result is same with the unskewed
    //s=3000 D1  misses:           41,701  (     26,947 rd   +      14,754 wr)
    //then it increases
	
    
        if (N > 7) {
            
            E2.resize(N + 1);
            H2.resize(N + 1);
         }
         int size= (sizeof(std::vector<float>) + (sizeof(float) * E2.size()))*2;
    	std::cout << "size of arrays= " << size << '\n';
    int count = 0;
    
    //simple FDTD calculations
    std::chrono::high_resolution_clock t1;    //initializing of the clock with the shortest tick period available 
    auto tstart=t1.now();                     //returns a std::chrono::time_point representing the current value of the clock
    
    
    //based on David Wonnacott
    //reference similar to that ftp://ftp.cs.rutgers.edu/cs/pub/technical-reports/work/dcs-tr-378.pdf
    //START OF THE TIME SKEWED CODE
    
    for (int ii = 0; ii < T + N; ii += s)
    for (int t = 0; t < T; ++t) {
        for (int i = std::max(t, ii); i < std::min(t + N, ii + s); ++i) {
            H2[i - t] += 0.5f*(E2[i - t +1 ] - E2[i - t]);        
        }
        for (int i = std::max(t +1, ii); i < std::min(t + N, ii + s); ++i) {
            E2[i - t] += 0.5f*(H2[i - t] - H2[i - t -1 ]);
            
        }
        
    }
    auto tend = t1.now();            //stopping the counter and calculating the duration
    auto dur = tend - tstart;
    std::cout << "duration1 = " << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << '\n';
    std::cout << "count= " << count << '\n';

}
