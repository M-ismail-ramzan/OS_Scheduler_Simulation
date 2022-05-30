#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <math.h>
#include <queue>
#include <sstream>
#include "os-kernel.h"
using namespace std;

int main(int argc, char *argv[])
{
    
    // An instance of the kernel
    Kernel spark_kernal;
    cout << "\n Checking for Argumnets \n";
    // checking for the erros at the arguments
    if (argc == 1)
    {
        cout << "\n Please Use the Following Format \n";
        cout << "\nos-kernel <Input file> <# CPUs> r <timeslice> <Output file>\n";
    }
    else
    {
        //reading the arguments
        for (int i = 0; i < argc; ++i)
        {
            // taking the Arguments
            if (i != 0)
                cout << argv[i] << "\n";
            
            
            if(i == 1){
                 // First Argumnet is going to be input file
                 spark_kernal.input_file_reader(argv[i]);
            }

        }
    }
    return 0;
}