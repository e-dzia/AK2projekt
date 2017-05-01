#include <stdio.h>
#include <sched.h>
#include <omp.h>
#include <iostream>

int fib(int n){
    if (n < 2)
        return 1;
    else return fib(n-1)+fib(n-2);
}

void lotsOfFib(){
    for(int i = 0; i < 100; i++){
        std::cout << char(omp_get_thread_num()+65) << fib(i) << " ";
    }
}

int main( int argc, char**argv )
{
#pragma omp parallel
    {
        printf( "Hello world from thread %d of %d running on cpu %2d!\n",
                omp_get_thread_num()+1,
                omp_get_num_threads(),
                1);
        lotsOfFib();

        //sched_getcpu()); //nie dziala, bo to kod na Linuxa :p
    }
    return 0;
}