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
#include <mutex>
//#include "os-kernel.h"
using namespace std;
// A PCB Struct to store the information if the processes
//#include "os-kernel.cpp"
 pthread_mutex_t mutex_locked_thread;
struct PCB
{
    // making the varibles for PCB
    string process_name;
    double priority;
    double arrival_time;
    string process_type;
    double cpu_time;
    double input_output_time;

    PCB()
    {
        // asssinging the initial values for safety lol
        process_name = "";
        priority = 0;
        arrival_time = 0;
        process_type = "";
        cpu_time = 0;
        input_output_time = 0;
    }

    // A function to assign the values of the file to the PCB
    void assign_values_to_pcb_variable(string process_info)
    {
        // Now time to break the string
        // Used for breaking words
        stringstream S(process_info);
        int index = 0;
        while (getline(S, process_info, ' '))
        {
            // if string is not empty
            if (process_info != "")
            {
                if (index == 0)
                {

                    // first col
                    this->process_name = (process_info);
                }
                else if (index == 1)
                {

                    // second col
                    this->priority = stod(process_info);
                }
                else if (index == 2)
                {

                    // third col
                    this->arrival_time = stod(process_info);
                }
                else if (index == 3)
                {

                    this->process_type = process_info;
                }
                else if (index == 4)
                {

                    this->cpu_time = stod(process_info);
                }
                else if (index == 5)
                {

                    this->input_output_time = stod(process_info);
                }
                else
                {

                    // there will be no column but still i added this for no reason
                }
                index++;
            }
        }
    }
    // Display all the PCB values
    void display_pcb_values()
    {
        cout << "\n"
             << this->process_name;
        cout << "\n"
             << this->priority;
        cout << "\n"
             << this->arrival_time;
        cout << "\n"
             << this->process_type;
        cout << "\n"
             << this->cpu_time;
        cout << "\n"
             << this->input_output_time;
    }
};

class Kernel
{
    // a pointer that can access the PCB anytime
    PCB *pcb_arr;
    int count_pcb_entry;
    // create the array of the threads id's
    

public:
    Kernel()
    {
        pcb_arr = new PCB[8];
        count_pcb_entry = 0;
    }
    void Implement_start(string file_name);
    void start_scheduler_with_threads(int cpu_cores);
    void fill_the_scheduler_queue();
    static void *helper_fill_the_scheduler_queue(void *p);
};

void Kernel::start_scheduler_with_threads(int cpu_cores)
{
    // This is time to start the Threads...

    pthread_t threads_id[cpu_cores];
    // create the corese as specified by the user
    for (int i = 0; i < cpu_cores; i++)
    {
        // creating the threads accoording to given time
        if (pthread_create(&threads_id[i], NULL, Kernel::helper_fill_the_scheduler_queue, NULL) != 0)
        {
            perror("\n Kernel Threads are unable to create");
            exit(0);
        }
    }

    for (int i = 0; i < cpu_cores; i++)
    {
        // creating the threads accoording to given time
        if (pthread_join(threads_id[i], NULL) != 0)
        {
            perror("\n Kernel Errror! in joining the Threads \n");
            exit(0);
        }
    }
}
void *Kernel::helper_fill_the_scheduler_queue(void *p)
{
    Kernel *a = (Kernel *)p;       
    a->fill_the_scheduler_queue(); 
    return NULL;                  
}
void Kernel::fill_the_scheduler_queue()
{
    sleep(1);
    // locked so one thread can write to the Queue at a time
    pthread_mutex_lock(&mutex_locked_thread);
    cout << " \nThread is Running : " << pthread_self() << "\n";
    // here we have to implement the Queue where the information will be transmitted
    sleep(1);
    pthread_mutex_unlock(&mutex_locked_thread);
    // Now there will be multiple threads running at a time
    // These threads will pass the information to the NEW Queue which will be then passed tp the scheduler
    // We must add lock in here because this area is being written by multiple thread

    // return NULL;
}

void Kernel::Implement_start(string file_name)
{
    cout << "\n Reading the File of the Processes \n";
    // Now I need to open that file and read all the information
    string process = "";
    // open the file
    fstream newfile;
    newfile.open("processes.txt");
    // now read information
    if (newfile.is_open())
    { // checking whether the file is open
        string tp;
        // keep track of the line no
        int i = 0;
        // read line by line from the string
        while (getline(newfile, tp))
        {
            // i Represent the Number of Process in the txt file
            if (i != 0)
            {
                // here we need to send the data to the structure
                cout << tp << "\n";
                // creaing a pcb for each process
                cout << "\n Counting " << count_pcb_entry;
                pcb_arr[count_pcb_entry].assign_values_to_pcb_variable(tp);
                count_pcb_entry++;
            }
            i++;
        }
        newfile.close(); // close the file object.
        cout << "\n File has been Read Successfully\n";
        // reading the PCB values for testing.
        for (int i = 0; i < count_pcb_entry; i++)
        {
            pcb_arr[i].display_pcb_values();
        }
    }
}

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
        // reading the arguments
        for (int i = 0; i < argc; ++i)
        {
            // taking the Arguments
            if (i != 0)
                cout << argv[i] << "\n";

            // First Argument
            if (i == 1)
            {
                // First Argumnet is going to be input file
                spark_kernal.Implement_start(argv[i]);
            }
            // Second Argument
            if (i == 2)
            {
                if (stoi(argv[i]) == 1 || stoi(argv[i]) == 2 || stoi(argv[i]) == 4)
                {
                    cout << "\n CPU is running with " << argv[i] << " Threads\n";
                    spark_kernal.start_scheduler_with_threads(stoi(argv[i]));
                }
                else
                {
                    cout << "\n Invalid Aurgumnets \n";
                    exit(1);
                }
            }
        }
    }
    return 0;
}