#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <queue>
#include <math.h>
#include <queue>
#include <sstream>
#include <mutex>
#include <list>
//#include "os-kernel.h"
using namespace std;
// A PCB Struct to store the information if the processes
//#include "os-kernel.cpp"
pthread_mutex_t mutex_locked_thread;
// For the Input Arguments
int CPU_CORES = 0;
char ALGO = 'f';
// Basic declearation to avoid ERROR'S
struct PCB;
class Processer;
class Scheduler
{

public:
    Processer *my_cpu;
    // these functions are just to Run the Threads for CPU
    void start_scheduler_with_threads(pthread_t *);
    static void *helper_fill_the_scheduler_queue(void *p);
    void fill_the_scheduler_queue(int cpu_cores);
    void send_new_queue_to_Ready_queue();
    void send_running_queue_to_waiting_queue(PCB);
    void send_waiting_queue_to_ready_queue();
    static void *helper_send_waiting_queue_to_ready_queue(void *p);
};
// Following are the Main Queues that will be used in this process
list<PCB> qlist;
queue<PCB> queue_new;
queue<PCB> queue_ready;
queue<PCB> queue_running;
queue<PCB> queue_waiting;
queue<PCB> queue_terminated;
// During the Execution we will be focusing on the Following Variables
int TOTAL_EXECUTION_TIME = 0;
int TOTAL_CONTEXT_SWICTING = 0;
int TOTAL_TIME_READY_STATE = 0;

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
    void assign_values_to_pcb_variable(string process_info, PCB &wow)
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
                    string tmp = "";
                    tmp = process_info;
                    this->priority = stod(tmp.c_str());
                }
                else if (index == 2)
                {

                    // third col
                    string tmp = "";
                    tmp = process_info;
                    this->arrival_time = stod(tmp);
                }
                else if (index == 3)
                {

                    this->process_type = process_info;
                }
                else if (index == 4)
                {

                    string tmp = "";
                    tmp = process_info;
                    this->cpu_time = stod(tmp);
                }
                else if (index == 5)
                {

                    string tmp = "";
                    tmp = process_info;
                    this->input_output_time = stod(tmp);
                }
                else
                {

                    // there will be no column but still i added this for no reason
                }
                index++;
            }
        }

        // checking if i really get the data xd
        qlist.push_back(wow);
        cout << "\n===========" << qlist.back().process_name << "\t\t" << qlist.back().priority << "\t\t" << qlist.back().arrival_time << "\t\t" << qlist.back().process_type << "\t\t" << qlist.back().cpu_time << "\t\t" << qlist.back().input_output_time << "\t\t"
             << "\n";
    }
    double get_arravil_time()
    {
        return arrival_time;
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
//-------------------------------END PCB-----------------------//

//------------------------------START OF PROCESSER------------------//
class Processer
{
public:
    // A Process PCB values will be sent to the Algo and the Algo will then Execute
    // the Process
    void Algo_First_come_First_server(PCB, Scheduler *);
};
// Take Pcb object and Run the Process
void Processer::Algo_First_come_First_server(PCB pcb_obj, Scheduler *scheduler_ptr)
{

    // Here we have to Perform the FCFS
    cout << "\n Process " << pcb_obj.process_name << " has To be Executed in " << pcb_obj.cpu_time << " with " << pcb_obj.input_output_time << " I/O";

    if (pcb_obj.input_output_time > 0)
    {
        // send the process to the Waiting Queue ///
        TOTAL_CONTEXT_SWICTING++;
        pcb_obj.input_output_time--;
        pcb_obj.cpu_time--;
        // send the Process to the waiting Queue
        // queue_waiting.push(pcb_obj);
        scheduler_ptr->send_running_queue_to_waiting_queue(pcb_obj);
    }
}

//-----------------------Scheduler--------------------------------//

void Scheduler::send_running_queue_to_waiting_queue(PCB obj)
{
    // This function will send the running Queue to the waiting Queue
    queue_waiting.push(queue_running.front());
    queue_running.pop();
}
void Scheduler::send_waiting_queue_to_ready_queue()
{
    // if waiting queue is empty then wait for something
    //  this will be a thread working in it's own zone

    // ceating a thread
    pthread_t waiting_queue_thread_id;
    if (pthread_create(&waiting_queue_thread_id, NULL, Scheduler::helper_send_waiting_queue_to_ready_queue, NULL) != 0)
    {
        perror("\n Kernel Threads are unable to create");
        exit(0);
    }

    // cout << "**************************************************8";
}

void *Scheduler::helper_send_waiting_queue_to_ready_queue(void *p)
{
    while (1)
    {
        if (!queue_waiting.empty())
        {
            
            Scheduler *ptr;
            cout << "\n RUNNING IN THE BACKGROUND FOR NO REASON";
            queue_waiting.pop();
            sleep(1);
            
        }
        else
        {
            sleep(1);
        }
    }
    return NULL;
}
void Scheduler::send_new_queue_to_Ready_queue()
{
    // first thing first start the working of backgrodun
    send_waiting_queue_to_ready_queue();
    // This function will send the new Queue to Ready according to Arrival Time
    // cout << "\n" << queue_new.front().process_name;

    while (!queue_new.empty())
    {
        cout << "\n NOTE: CPU is running with " << CPU_CORES << " Threads\n";
        cout << "\n--------READY QUEUE-----\n";
        cout << "\n Process " << queue_new.front().process_name << " Arrived \n";
        queue_ready.push(queue_new.front());
        queue_new.pop();
        // Now once we get the Process into the Ready Queue Now it's time to execute it....
        cout << "\n-----------------------\n";
        sleep(queue_new.front().arrival_time);
    }
}
void Scheduler::start_scheduler_with_threads(pthread_t *threads_id)
{

    // create the corese as specified by the user
    for (int i = 0; i < CPU_CORES; i++)
    {
        void *ptr = &CPU_CORES;
        // creating the threads accoording to given time
        if (pthread_create(&threads_id[i], NULL, Scheduler::helper_fill_the_scheduler_queue, ptr) != 0)
        {
            perror("\n Kernel Threads are unable to create");
            exit(0);
        }
    }
}

void *Scheduler::helper_fill_the_scheduler_queue(void *p)
{
    Scheduler *a; //= (Kernel *)p;
    int *x = (int *)p;
    a->fill_the_scheduler_queue(*x);
    return NULL;
}
void Scheduler::fill_the_scheduler_queue(int cpu_cores)
{
    while (1)
    {
        pthread_mutex_lock(&mutex_locked_thread);
        sleep(1);
        // locked so one thread can write to the Queue at a time
        cout << "\nThread waiting: " << pthread_self() << "\n";
        while (queue_ready.empty())
            ;
        cout << " \nThread is Running : " << pthread_self() << "\n";
        // get the PCB and give it to the Processer
        PCB tmp_obj = queue_ready.front();
        queue_ready.pop();
        if (ALGO == 'f')
        {
            cout << "Running the FCFS";
            // GETTING TO THE RUNNING STATE
            TOTAL_CONTEXT_SWICTING++;
            queue_running.push(tmp_obj);
            my_cpu->Algo_First_come_First_server(tmp_obj, this);
        }
        // priority_queue<PCB> queue_new;
        // Now we have to push into the queue using the arriaval time
        // here we have to implement the Queue where the information will be transmitted
        pthread_mutex_unlock(&mutex_locked_thread);
        // return NULL;
    }
}
//-------------------------------End Scheduler-----------------------//

class Kernel
{
    // a pointer that can access the PCB anytime
    PCB pcb_arr;
    int count_pcb_entry;
    // create the array of the threads id's
    // This is time to start the Threads...

public:
    Scheduler *kernel_scheduler;
    Kernel()
    {
        // pcb_arr = new PCB[NO_OF_PROCESSES];
        count_pcb_entry = 0;
        kernel_scheduler = new Scheduler;
    }
    void Implement_start(string file_name);
    void controller_thread(int cpu_cores, pthread_t *);
    void start_scheduler_with_threads(int cpu_cores);
    void showlist(list<PCB> g);
    void send_list_to_queue_new(list<PCB>);
};
void Kernel::controller_thread(int cpu_cores, pthread_t *threads_id)
{
    // now we will Populate the New Queue
    send_list_to_queue_new(qlist);
    // display the queue
    cout << "\n The data Currently into the New Queue \n";
    cout << "\n-----------------------------------------\n";
    queue<PCB> tmp_q = queue_new; // copy the original queue to the temporary queue

    while (!tmp_q.empty())
    {
        // Just for Showing the Data into the Queue
        string name = tmp_q.front().process_name;
        float prorirty = tmp_q.front().priority;
        float arri_time = tmp_q.front().arrival_time;
        string pro_type = tmp_q.front().process_type;
        float cpu_time = tmp_q.front().cpu_time;
        float in_ou_time = tmp_q.front().input_output_time;
        std::cout << name << "\t\t" << prorirty
                  << "\t" << arri_time << "\t" << pro_type << "\t" << cpu_time << "\t" << in_ou_time << "\t"
                  << "\n";
        tmp_q.pop();
    }

    cout << "\n-----------------------------------------\n";

    cout << "\n Controller Thread will Now Ask Scheduler (Short term) to Send the Processes to the Ready Queue According to their Arrival Time \n";
    ;

    // We can either have 4,2,1 CPU's Remember THAT!
    kernel_scheduler->start_scheduler_with_threads(threads_id);
    kernel_scheduler->send_new_queue_to_Ready_queue();

    // After u are done working jOIN THE Threads...
    for (int i = 0; i < CPU_CORES; i++)
    {
        // creating the threads accoording to given time
        if (pthread_join(threads_id[i], NULL) != 0)
        {
            perror("\n Kernel Errror! in joining the Threads \n");
            exit(0);
        }
    }
}

// function for printing the elements in a list
void Kernel::showlist(list<PCB> g)
{
    PCB test;
    list<PCB>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
    {
        test = *it;

        remove(test.process_name.begin(), test.process_name.end(), ' ');
        remove(test.process_type.begin(), test.process_type.end(), ' ');
        std::cout << "\n"
                  << test.process_name << "\t" << test.priority
                  << "\t" << test.arrival_time << "\t" << test.process_type << "\t" << test.cpu_time << "\t" << test.input_output_time << "\t";
    }
    cout << "\t";
    cout << "\n";
}
void Kernel::send_list_to_queue_new(list<PCB> g)
{
    PCB test;
    list<PCB>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
    {
        // Put the Linked list into the Queue
        test = *it;
        cout << test.input_output_time << "\t";
        queue_new.push(test);
    }
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
                // cout << tp << "\n";
                // creaing a pcb for each process
                // cout << "\n Counting " << count_pcb_entry << "\n";

                pcb_arr.assign_values_to_pcb_variable(tp, pcb_arr);
                count_pcb_entry++;
            }
            i++;
        }
        newfile.close(); // close the file object.
        cout << "\n File has been Read Successfully\n";
        showlist(qlist);
    }
}

//------------------------MAIN CODE--------------------------------//

int main(int argc, char *argv[])
{

    // An instance of the kernel
    Kernel spark_kernal;
    cout << "\n Checking for Argumnets \n";
    // checking for the erros at the arguments
    if (argc < 5)
    {
        cout << "\n Please Use the Following Format \n";
        cout << "\nos-kernel <Input file> <# CPUs> r <timeslice> <Output file>\n";
        cout << "\nos-kernel <Input file> <# CPUs> f <Output file>\n";
        cout << "\nos-kernel <Input file> <# CPUs> p <Output file>\n";
    }
    else
    {
        cout << argv[3];
        // checking some argument..

        if (!(*(argv[3]) == 'f' || *(argv[3]) == 'p' || *(argv[3]) == 'r'))
        {
            cout << "\n Invalid Aurgumnets \n";
            exit(1);
        }
        else
        {
            // this is the Algo we are going to RUNnnn
            ALGO = *(argv[3]);
        }
        // reading the arguments
        for (int i = 0; i < argc; ++i)
        {
            // taking the Arguments

            // cout << argv[i] << "\n";

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

                    CPU_CORES = stoi(argv[i]);
                    pthread_t threads_id[CPU_CORES];
                    spark_kernal.controller_thread(stoi(argv[i]), threads_id);

                    // spark_kernal.start_scheduler_with_threads(stoi(argv[i]));
                }
                else
                {
                    cout << "\n Invalid Aurgumnets \n";
                    exit(1);
                }
            }
            // Second Argument
            if (i == 3)
            {
            }
        }
    }
    return 0;
}