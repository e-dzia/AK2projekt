#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "TimerFactory.h"
#include "Utils.h"

void gauss(int n, double ** AB, double * X);
void gaussParallel(int n, double ** AB, double * X);
void pivot(int n, double **AB, int j);

int main()
{
    int numberOfIterations = 50;

    double **AB = nullptr;
    double *X = nullptr;
    double timeNormal, timeParallel;
    int n, i, j, k;
    int sizes[] = {100,200,500,1000};
    int sizeOfSizes = sizeof(sizes)/sizeof(sizes[0]);
    Timer *timer = TimerFactory::createTimer();

    std::ofstream fout;
    fout.open("results.txt");

    n = 10;

    for (i = 0; i < sizeOfSizes; i++) {
        n = sizes[i]; //number of rows
        for (j = 0; j < numberOfIterations; j++) {
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
    //elimination
    for (int i = 0; i < n-1;  i++){
        for (int j = i+1; j < n; j++){
            double xFac = -AB[j][i]/AB[i][i];
            for (int k = i+1; k < n+1; k++){
                AB[j][k] += xFac*AB[i][k];
            }
        }
    }

    //count X
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
    double xFac;
    int i, j, k;

    //elimination
    for (j = 0; j < n-1; j++){
        pivot(n, AB, j);

#pragma omp parallel for default(none) shared(n,AB,j) private(i,k,xFac)
        for (k = j+1; k < n; k++){
            xFac = -AB[k][j]/AB[j][j];
            for (i = j; i < n+1; i++){
                AB[k][i] += xFac * AB[j][i];
            }
        }
    }

    //count X
    for(i = n - 1; i >= 0; i--)
    {
        double s = AB[i][n];
#pragma omp parallel for default(none) private(j) shared(AB,X,n,i,s)
        for(j = n - 1; j >= i + 1; j--){
            s -= AB[i][j] * X[j];
        }
        X[i] = s / AB[i][i];
    }
}


void pivot(int n, double **AB, int j)
{

    int   i, k, m;
    double xFac, tmp, aMax;

    aMax = fabs(AB[j][j]) ;
    m = j;
    //find the row with largest pivot
#pragma omp parallel for default(none) shared(n,AB,j,aMax,m) private(xFac,i)
    for (i = j+1; i < n; i++){
        xFac = fabs(AB[i][j]);
        if(xFac > aMax) {
            aMax = xFac;
            m = i;
        }
    }

    //row interchanges
    if(m != j) {
#pragma omp parallel for default(none) shared(n,AB,j,m) private(k,tmp)
        for(k = j; k < n+1; k++) {
            tmp = AB[j][k];
            AB[j][k] = AB[m][k];
            AB[m][k] = tmp;
        }
    }
}
