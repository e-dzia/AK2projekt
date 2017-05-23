#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "Timer.h"
#include "TimerFactory.h"
#include "Utils.h"

bool gauss(int n, double ** AB, double * X);
bool gaussParallel(int n, double ** AB, double * X);

int main()
{
    double **AB, *X;
    int      n;
    int sizes[] = {100,200,500,1000,2000};
    int sizeOfSizes = sizeof(sizes)/sizeof(sizes[0]);
    std::cout << sizeOfSizes << std::endl;

    std::ofstream fout; fout.open("results.txt");

    for (int i = 0; i < sizeOfSizes; i++){
        n = sizes[i]; //number of rows
        std::cout << i << " " << n << " " << sizeOfSizes << std::endl;
        for (int j = 0; j < 10; j++){
            std::cout << j << std::endl;
            X  = new double [n];
            AB = nullptr;

            generateNumbers(AB,n,n+1);
            saveToFile("data.txt", AB, n, n + 1);

            Timer *timer = TimerFactory::createTimer();
            timer->start();
            gauss(n,AB,X);
            timer->stop();

            double timeNormal = timer->get();

            readFromFile("data.txt",AB,n,n+1);

            timer->start();
            gaussParallel(n,AB,X);
            timer->stop();

            double timeParallel = timer->get();

            fout << n << " " << timeNormal << " " << timeParallel << std::endl;

            for(int k = 0; k < n; k++) delete [] AB[k];
            delete [] AB;
            delete [] X;
        }
    }

    return 0;
}

bool gauss(int n, double ** Ab, double * X)
{
    //eliminowanie wspolczynnikow
    for (int i = 0; i < n-1;  i++){
        for (int j = i+1; j < n; j++){
            double xFac = -Ab[j][i]/Ab[i][i];
            for (int k = i+1; k < n+1; k++){
                Ab[j][k] += xFac*Ab[i][k];
            }
        }
    }

    //wyliczanie X
    for(int i = n - 1; i >= 0; i--)
    {
        double s = Ab[i][n];
        for(int j = n - 1; j >= i + 1; j--){
            s -= Ab[i][j] * X[j];
        }
        X[i] = s / Ab[i][i];
    }

}

bool gaussParallel(int n, double ** Ab, double * X)
{
    int i, j, k;
    //eliminowanie wspolczynnikow
#pragma omp parallel for private(i,j,k) shared(Ab,n)
    for (i = 0; i < n-1;  i++){
        for (j = i+1; j < n; j++){
            double xFac = -Ab[j][i]/Ab[i][i];
            for (k = i+1; k < n+1; k++){
                Ab[j][k] += xFac*Ab[i][k];
            }
        }
    }

    //wyliczanie X
#pragma omp parallel for private(i,j) shared(Ab,X,n)
    for(i = n - 1; i >= 0; i--)
    {
        double s = Ab[i][n];
        for(j = n - 1; j >= i + 1; j--){
            s -= Ab[i][j] * X[j];
        }
        X[i] = s / Ab[i][i];
    }

}