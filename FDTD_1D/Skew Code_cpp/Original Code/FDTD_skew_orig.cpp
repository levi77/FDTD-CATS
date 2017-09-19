//TIME SKEWING CODE
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

    const int T = 6; //6 for very small test of above EH fields//number of time steps
    const int N = 7;//7 for very small test//FDTD 1D number of cells
    const int s = 2; //2 for very small test //tile width
	//T = 1000;N = 1000000;s = 50000;
        if (N > 7) {
            E1.resize(N + 1);
            H1.resize(N + 1);
            E2.resize(N + 1);
            H2.resize(N + 1);
         }
    int count = 0;
    
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
        //            std::cout << "i= " << i << "  t=" << t << "     " << i + 1 << " = " << i << ", " << i + 1 << ", " << i + 2 << '\n';
        //        E1[4] = 10;
        //            count++;
    }
    auto tend = t1.now();            //stopping the counter and calculating the duration
    auto dur = tend - tstart;
    std::cout << "duration1 = " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << '\n'; //outputs of the normal FDTD CODE
    std::cout << "count= " << count << '\n';
    count = 0;
    int licznik = 1;
    //based on David Wonnacott
    //reference similar to that ftp://ftp.cs.rutgers.edu/cs/pub/technical-reports/work/dcs-tr-378.pdf
    //START OF THE TIME SKEWED CODE
    tstart = t1.now();
    for (int ii = 0; ii < T + N; ii += s)
    for (int t = 0; t < T; ++t) {
        for (int i = std::max(t, ii); i < std::min(t + N, ii + s); ++i) {
            H2[i - t] += 0.5f*(E2[i + 1 - t] - E2[i - t]);
            //                std::cout << "licznik = " <<licznik << " i = " << i << "  t=" << t << "     " << "H2[" << i - t << "] += " << "0.5*( E2["<<i+1 - t << "] -E2[" << i  - t << "] )" << '\n';;
            //                licznik++;
        }
        #ifdef OK
        //Attention imine and imaxe are 1 less than iminh i imaxh
        const int imine = (iminh - 1 < 0) ? 0 : iminh - 1;
        const int imaxe = imaxh - 1;
        for (int i = imine; i < imaxe; ++i) {
            if ((i - t) < 0) continue; //Attention additional operation/chack to have index greater than 0 for H2[i - t] -
            E2[i +1 - t] += 0.5*(H2[i + 1 - t] - H2[i - t]);
            //                std::cout << "counter = " << counter << " i= " << i << "  t=" << t << "     " << "E2[" << i + 1 - t << "] += " << "0.5*( H2[" << i + 1 - t << "] -H2[" << i - t << "] )" << '\n';
            counter++;
        }
        #endif
        for (int i = std::max(t +1, ii); i < std::min(t + N, ii + s); ++i) {
            E2[i - t] += 0.5f*(H2[i - t] - H2[i - t -1 ]);
            //                std::cout << "counter = " << counter << " i= " << i << "  t=" << t << "     " << "E2[" << i + 1 - t << "] += " << "0.5*( H2[" << i + 1 - t << "] -H2[" << i - t << "] )" << '\n';
            //                counter++;
        }
        //                std::cout << "i= " << i << " t=" << t << "     " << i + 1 - t << " = " << i - t << ", " << i + 1 - t << ", " << i + 2 - t << '\n';;
        //            E2[0] += 0.5*(H2[0]);
        //                count++;
    }
    tend = t1.now();
    dur = tend - tstart;
    std::cout << "duration2 = " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << '\n';
    std::cout << "count= " << count << '\n';
    if (H1==H2 && E1==E2)
    std::cout << "Vectors H1 i E1 are equal to H2 E2 - simple algorithm and time skewing have the same results\n";
    else
    std::cout << "simple algorithm and time skewing have different results\n";
    std::cin.get();
}
