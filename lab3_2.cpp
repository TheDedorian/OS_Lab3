#include <iostream>
#include <windows.h>
#include <omp.h>

using namespace std;

#define N 100000000
#define BlockSize 9307060
double InvertedN = (1.0/N);
#define ThreadCount 1


int main()
{
    double pi = 0,
    x,
    operation;
    
    DWORD start_time = GetTickCount();
    #pragma omp parallel for num_threads(ThreadCount) shedule(dynamic, BlockSize) reduction(+:pi) private(x, operation) //reduction позволит
    //выполнить оператор "+" над всеми локальными копиями pi

    for (DWORD i = 0;i < N;i++) {
        x = (i + 0.5) * InvertedN;
        operation = (4.0 / (1.0 + x*x));
        pi += operation;
    }
    
    pi = pi*InvertedN;
    DWORD end_time = GetTickCount();
    cout.precision(10);
    cout << "Pi: " << pi << endl;
    cout << "Time: " << (double)(end_time-start_time)/1000 << endl;
    while(1);
}