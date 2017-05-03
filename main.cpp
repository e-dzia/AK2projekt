#include <stdio.h>
#include <sched.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Timer.h"
#include "TimerWindows.h"

const double eps = 1e-12;

bool gauss(int n, double ** AB, double * X);
bool gaussParallel(int n, double ** AB, double * X);
bool createFile(int n);

int main()
{
    double **AB, *X;
    int      n,i,j;

    std::cout << std::setprecision(4) << std::fixed;

    n = 2000;
    createFile(n);
    std::ifstream fin;
    fin.open("data.txt", std::ios::in);
    fin >> n;

    // tworzymy macierze AB i X
    AB = new double * [n];
    X  = new double [n];

    for(i = 0; i < n; i++)
        AB[i] = new double[n + 1];

    // odczytujemy dane dla macierzy AB
    for(i = 0; i < n; i++)
        for(j = 0; j <= n; j++)
            fin >> AB[i][j];
    fin.close();

    /*if(gauss(n,AB,X))
    {
        for(i = 0; i < n; i++)
            std::cout << "x" << i + 1 << " = " << std::setw(9) << X[i]
                 << std::endl;
    }
    else
        std::cout << "DZIELNIK ZERO\n";
*/

    Timer *timer = new TimerWindows();
    timer->start();
    gauss(n,AB,X);
    timer->stop();

    double time1 = timer->get();

    std::cout << "Noelo\n";

    fin.open("data.txt", std::ios::in);
    fin >> n;;

    // odczytujemy dane dla macierzy AB
    for(i = 0; i < n; i++)
        for(j = 0; j <= n; j++)
            fin >> AB[i][j];
    fin.close();

    timer->start();
    gaussParallel(n,AB,X);
    timer->stop();

    double time2 = timer->get();

    std::cout << time1 << " " << time2 << std::endl;

    // usuwamy macierze z pamięci
    for(i = 0; i < n; i++) delete [] AB[i];
    delete [] AB;
    delete [] X;

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

bool createFile(int n) {
    std::ofstream fout;
    fout.open("data.txt", std::ios::out);
    if (fout.is_open()){
        fout << n << std::endl;
        for (int i = 0; i < n; i++){
            for (int j = 0; j <= n; j++){
                int length = rand()%10+1;
                fout << length << " ";
            }
            fout << std::endl;
        }
        fout.close();
        return true;
    }
    else return false;
}
