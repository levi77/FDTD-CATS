//TIME SKEWING CODE
//g++ -g -Wall -std=c++11  FDTD_skew.cpp -o FDTD_skew
//valgrind --tool=cachegrind --cachegrind-out-file=outfdtd2 ./FDTD2
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
//TEST FDTD time skewing
int main() {
    
    std::vector<float> E1 = { 0, 0, 0, 0, 10, 0, 0, 0, 0 };
    std::vector<float> H1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<float> E2 = { 0, 0, 0, 0, 10, 0, 0, 0, 0 };
    std::vector<float> H2 = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    
    
    const int T = 100; //6 for very small test of above EH fields//number of time steps
    const int N = 10000000;//7 for very small test//FDTD 1D number of cells
    const int s = 10000; //2 for very small test //tile width 50000
    int size2= sizeof(s);
    std::cout << "size of = " << size*2 << '\n';
    	//T = 100 ;N = 100000;s = 10000;
        if (N > 7) {
            E1.resize(N + 1);
            H1.resize(N + 1);
            E2.resize(N + 1);
            H2.resize(N + 1);
         }
    int count = 0;
    int size= sizeof(std::vector<float>) + (sizeof(float) * E1.size());
    std::cout << "size of arrays= " << size*2 << '\n';
    //simple FDTD calculations
    std::chrono::high_resolution_clock t1;    //initializing of the clock with the shortest tick period available 
    auto tstart=t1.now();                     //returns a std::chrono::time_point representing the current value of the clock
    
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
    //printing out arrays
    //for (std::vector<float>::const_iterator i = E1.begin(); i != E1.end(); ++i)
    //std::cout << *i << ' ';
    auto tend = t1.now();            //stopping the counter and calculating the duration
    auto dur = tend - tstart;
    std::cout << "\n duration1 = " << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << '\n'; //outputs of the normal FDTD CODE
    //std::cout << "count= " << count << '\n';
    count = 0;
    
    //based on David Wonnacott
    //reference similar to that ftp://ftp.cs.rutgers.edu/cs/pub/technical-reports/work/dcs-tr-378.pdf
    //START OF THE TIME SKEWED CODE
    tstart = t1.now();
    for (int ii = 0; ii < T + N; ii += s)
		for (int t = 0; t < T; ++t) {
		    for (int i = std::max(t, ii); i < std::min(t + N, ii + s); ++i) {
		        H2[i - t] += 0.5f*(E2[i - t+ 1] - E2[i - t]);        
		    }
		    for (int i = std::max(t +1, ii); i < std::min(t + N, ii + s); ++i) {
		        E2[i - t] += 0.5f*(H2[i - t] - H2[i - t -1 ]);
		        
		    }
		    
		}
	//printing out arrays
	//for (std::vector<float>::const_iterator i = E2.begin(); i != E2.end(); ++i)
    //std::cout << *i << ' ';
    tend = t1.now();
    dur = tend - tstart;
    std::cout << "\n duration2 = " << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << '\n';
    //std::cout << "count= " << count << '\n';
    
    if (H1==H2 && E1==E2)
    	std::cout << "Vectors H1 i E1 are equal to H2 E2 - simple algorithm and time skewing have the same results\n";
    else
    	std::cout << "simple algorithm and time skewing have different results\n";
}
