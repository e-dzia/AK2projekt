#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "TimerFactory.h"
#include "Utils.h"

void gauss(int n, double ** AB, double * X);
void gaussParallel(int n, double ** AB, double * X);

int main()
{
    double **AB = nullptr;
    double *X = nullptr;
    double timeNormal, timeParallel;
    int n, i, j, k;
    int sizes[] = {100,200,500,1000,2000};
    int sizeOfSizes = sizeof(sizes)/sizeof(sizes[0]);
    Timer *timer = TimerFactory::createTimer();

    std::cout << sizeOfSizes << std::endl;

    std::ofstream fout;
    fout.open("results.txt");

    for (i = 0; i < sizeOfSizes; i++) {
        n = sizes[i]; //number of rows
        std::cout << i << " " << n << " " << sizeOfSizes << std::endl;
        for (j = 0; j < 10; j++) {
            std::cout << j << std::endl;
            X = new double [n];

            generateNumbers(AB, n, n + 1);
            saveToFile("data.txt", AB, n, n + 1);

            timer->start();
            gauss(n,AB,X);
            timer->stop();
            timeNormal = timer->getSeconds();

            readFromFile("data.txt", AB, n, n+1);

            timer->start();
            gaussParallel(n,AB,X);
            timer->stop();
            timeParallel = timer->getSeconds();

            fout << n << " " << timeNormal << " " << timeParallel << std::endl;

            for(k = 0; k < n; ++k) delete [] AB[k];
            delete [] AB;
            delete [] X;
        }
    }

    return 0;
}

void gauss(int n, double ** AB, double * X)
{
    //eliminowanie wspolczynnikow
    for (int i = 0; i < n-1;  i++){
        for (int j = i+1; j < n; j++){
            double xFac = -AB[j][i]/AB[i][i];
            for (int k = i+1; k < n+1; k++){
                AB[j][k] += xFac*AB[i][k];
            }
        }
    }

    //wyliczanie X
    for(int i = n - 1; i >= 0; i--)
    {
        double s = AB[i][n];
        for(int j = n - 1; j >= i + 1; j--){
            s -= AB[i][j] * X[j];
        }
        X[i] = s / AB[i][i];
    }

}

void gaussParallel(int n, double ** AB, double * X)
{
    int i, j, k;
    //eliminowanie wspolczynnikow
#pragma omp parallel for private(i,j,k) shared(AB,n)
    for (i = 0; i < n-1;  i++){
        for (j = i+1; j < n; j++){
            double xFac = -AB[j][i]/AB[i][i];
            for (k = i+1; k < n+1; k++){
                AB[j][k] += xFac*AB[i][k];
            }
        }
    }

    //wyliczanie X
#pragma omp parallel for private(i,j) shared(AB,X,n)
    for(i = n - 1; i >= 0; i--)
    {
        double s = AB[i][n];
        for(j = n - 1; j >= i + 1; j--){
            s -= AB[i][j] * X[j];
        }
        X[i] = s / AB[i][i];
    }

}