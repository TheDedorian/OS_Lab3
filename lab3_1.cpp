#include <iostream>
#include <windows.h>

using namespace std;

#define N 100000000
#define BlockSize 9307060
double InvertedN = (1.0/N);
#define ThreadCount 16
long NumberBlockObrAll = -1;
double pi = 0;
HANDLE hMutex;


DWORD WINAPI Sync(LPVOID parametr) {
    int counter = 0;
    double ThreadPi = 0;
    double x;
    DWORD SizeProgression;
    while(1) {
        InterlockedIncrement(&NumberBlockObrAll);
        counter = NumberBlockObrAll*BlockSize;
        if(counter + BlockSize < N)
            SizeProgression = BlockSize + counter;
        else
            SizeProgression = N;
        for(DWORD i = counter; i < SizeProgression ; i++){
            x = ( i + 0.5) * InvertedN;
            ThreadPi += (4.0/(1.0 + x*x));
        }
        if(counter > N) {
            WaitForSingleObject(hMutex, INFINITE);
            pi += ThreadPi;
            ReleaseMutex(hMutex);
            return 0;
        }
    }
}

int main()
{
    HANDLE Threads[ThreadCount];
    DWORD start_time;
    DWORD end_time;
    HANDLE hMutex = CreateMutex(NULL, FALSE, "");

    for(int k = 0; k<ThreadCount; k++){
        Threads[k] = CreateThread(NULL, 0, Sync, NULL, CREATE_SUSPENDED, NULL);
    }

    start_time = GetTickCount();

    for(int k = 0; k<ThreadCount; k++){
        ResumeThread(Threads[k]);
        if(NULL == Threads[k]) cout << "Error Create!" << endl;
    }

    WaitForMultipleObjects(ThreadCount, Threads, TRUE, INFINITE);

    pi = 4*pi*InvertedN;
    end_time = GetTickCount();
    cout.precision(10);
    cout << "Pi: " << pi << endl;
    cout << "Time: " << (double)(end_time-start_time)/1000 << endl;

    CloseHandle(hMutex);
    return 0;
}