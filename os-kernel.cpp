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
#include <iomanip>
#include <semaphore.h>
#include <sstream>
#include <fcntl.h>
//#include "os-kernel.h"
using namespace std;
// A PCB Struct to store the information if the processes
//#include "os-kernel.cpp"
pthread_mutex_t mutex_locked_thread;
pthread_mutex_t mutex_locked_thread1;
// using samphore for multithreading
sem_t samphore_multithreading;

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
    void terminate_the_process_to_terminated_queue(PCB obj);
    void send_running_queue_to_ready_queue(PCB obj);
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
int TIME_SLICE = 1;
pthread_t kernel_print_output;
priority_queue<PCB> queue_ready_proprity;
char *OUTPUT_FILE_NAME;
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

    bool operator<(const PCB &rhs) const
    {
        return priority < rhs.priority;
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
void *helper_Print_Output(void *p)
{
    // This function will also Run the Background and will Display the Output...
    // FIRST COLUMN WILL PRINT THE TIME
    // SECOND COLUMN THE LENGTH OF QUEUE'S
    // THIRD COLUMN NAME OF PROCESS that each CPU is executing
    // FOURTH COLUMN SHOWS THE I/O QUEUE PROCESS NAME....
    int cout_descriptor = dup(STDOUT_FILENO);
    while (1)
    {
        dup2(cout_descriptor, STDOUT_FILENO);
        // if all the queues are empty it means program is terminated

        if ((queue_new.empty() && queue_ready.empty() && queue_waiting.empty() && queue_running.empty()))
        {
            cout << "\nTOTAL CONTEXT SWITCHING : " << TOTAL_CONTEXT_SWICTING << endl;
            cout << "\nTOTAL EXECUTION TIME : " << TOTAL_EXECUTION_TIME << endl;
            cout << "\nTOTAL READY STATE : " << TOTAL_TIME_READY_STATE << endl;
            exit(0);
        }

        //  cout << "\n Inside the thread \n";

        //   cout << "I am causing Problems: " << pthread_self();
        switch (CPU_CORES)
        {
        case 1:
        {

            cout << "\n\n";
            cout << "\n -----------------------------ORIGINAL OUTPUT--------------------------\n";
            cout << setw(10) << "Time" << setw(10) << "RU" << setw(10) << "RE" << setw(10) << "WA" << setw(10) << "CPU" << setw(20) << "I/O"
                 << "\n";
            static int time = 0;
            time++;

            string cpu_process_running = "NULL";
            string io_process_running = "NULL";
            // checking if qeue is empty or not
            if (!queue_running.empty())
            {
                cpu_process_running = queue_running.front().process_name;
            }
            // checking if the queue waiting is not empty
            if (!queue_waiting.empty())
            {
                io_process_running = queue_waiting.front().process_name;
            }

            // display on terminal
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running << setw(20) << io_process_running
                 << "\n";
            cout << "\n\n";
            // Now change the Descriptor
            // Open up a file
            int file = open(OUTPUT_FILE_NAME, O_APPEND | O_WRONLY | O_CREAT, 0777);
            if (file == -1)
            {
                cout << "\nERROR IN FILE OPENNIG\n";
            }
            dup2(file, STDOUT_FILENO);
            // Save on the file
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running << setw(20) << io_process_running;
            close(file);
            //  cout << "am not running duudeee";
            sleep(2);
            break;
        }
        case 2:
        {

            cout << "\n\n";
            cout << "\n -----------------------------ORIGINAL OUTPUT--------------------------\n";
            cout << setw(10) << "Time" << setw(10) << "RU" << setw(10) << "RE" << setw(10) << "WA" << setw(10) << "CPU 1" << setw(10) << "CPU 2" << setw(20) << "I/O"
                 << "\n";
            static int time = 0;
            time++;

            string cpu_process_running = "NULL";
            string io_process_running = "NULL";
            // checking if qeue is empty or not
            if (!queue_running.empty())
            {
                cpu_process_running = queue_running.front().process_name;
            }
            // checking if the queue waiting is not empty
            if (!queue_waiting.empty())
            {
                io_process_running = queue_waiting.front().process_name;
            }
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running << setw(10) << cpu_process_running << setw(20) << io_process_running
                 << "\n";

            // Open up a file
            int file = open(OUTPUT_FILE_NAME, O_APPEND | O_WRONLY | O_CREAT, 0777);
            if (file == -1)
            {
                cout << "\nERROR IN FILE OPENNIG\n";
            }
            dup2(file, STDOUT_FILENO);
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running << setw(10) << cpu_process_running << setw(20) << io_process_running
                 << "\n";
            // Save on the file
            close(file);
            // sleep(1);
            cout << "\n\n";
            //  cout << "am not running duudeee";
            sleep(2);
            break;
        }
        case 4:
        {
            cout << "\n\n";
            cout << "\n -----------------------------ORIGINAL OUTPUT--------------------------\n";
            cout << setw(10) << "Time" << setw(10) << "RU" << setw(10) << "RE" << setw(10) << "WA" << setw(10) << "CPU 1" << setw(10) << "CPU 2" << setw(10) << "CPU 3" << setw(10) << "CPU 4" << setw(20) << "I/O"
                 << "\n";
            static int time = 0;
            time++;

            string cpu_process_running_1 = "NULL";
            string cpu_process_running_2 = "NULL";
            string cpu_process_running_3 = "NULL";
            string cpu_process_running_4 = "NULL";
            string io_process_running = "NULL";
            // checking if qeue is empty or not
            if (!queue_running.empty())
            {
                queue<PCB> temp;
                temp = queue_running;

                cpu_process_running_1 = temp.front().process_name;
                temp.pop();
                if (!temp.empty())
                {
                    cpu_process_running_2 = temp.front().process_name;
                    temp.pop();
                }
                if (!temp.empty())
                {
                    cpu_process_running_3 = temp.front().process_name;
                    temp.pop();
                }
                if (!temp.empty())
                {
                    cpu_process_running_4 = temp.front().process_name;
                    temp.pop();
                }
            }
            // checking if the queue waiting is not empty
            if (!queue_waiting.empty())
            {
                io_process_running = queue_waiting.front().process_name;
            }
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running_1 << setw(10) << cpu_process_running_2 << setw(10) << cpu_process_running_3 << setw(10) << cpu_process_running_4 << setw(20) << io_process_running
                 << "\n";

            // Open up a file
            int file = open(OUTPUT_FILE_NAME, O_APPEND | O_WRONLY | O_CREAT, 0777);
            if (file == -1)
            {
                cout << "\nERROR IN FILE OPENNIG\n";
            }
            dup2(file, STDOUT_FILENO);
            cout << setw(10) << time << setw(10) << queue_running.size() << setw(10) << queue_ready.size() << setw(10) << queue_waiting.size() << setw(10) << cpu_process_running_1 << setw(10) << cpu_process_running_2 << setw(10) << cpu_process_running_3 << setw(10) << cpu_process_running_4 << setw(20) << io_process_running
                 << "\n";
            close(file);
            // sleep(1);
            cout << "\n\n";
            //  cout << "am not running duudeee";
            sleep(2);
            break;
        }

        default:
            break;
        }
    }
    return NULL;
}

//------------------------------START OF PROCESSER------------------//
class Processer
{
public:
    // A Process PCB values will be sent to the Algo and the Algo will then Execute
    // the Process
    void Algo_First_come_First_server(PCB, Scheduler *);
    void Algo_Round_Robin(PCB, Scheduler *);
    void Algo_Premitive_Priority(PCB, Scheduler *);
};
void Processer::Algo_Round_Robin(PCB pcb_obj, Scheduler *scheduler_ptr)
{

    // Here we have to PERFORM ROUND ROBIN YAYAYA
    cout << "\n Process " << pcb_obj.process_name << " has To be Executed in " << pcb_obj.cpu_time << " with " << pcb_obj.input_output_time << " I/O";

    if (pcb_obj.input_output_time > 0)
    {
        // send the process to the Waiting Queue ///
        TOTAL_CONTEXT_SWICTING++;
        // adding 2 cuz
        // -- CPU TIME + I/O time (Assume 1 sec)
        //     So,    1 + 1 = 2
        TOTAL_EXECUTION_TIME = TOTAL_EXECUTION_TIME + 2;
        // drcreasing the i/o by one because it' going for input
        pcb_obj.input_output_time = (pcb_obj.input_output_time) - 1;
        // just assuming it will go to i/o after 1 sec
        pcb_obj.cpu_time = (pcb_obj.cpu_time) - 1;
        // send the Process to the waiting Queue
        // queue_waiting.push(pcb_obj);
        pthread_mutex_lock((&mutex_locked_thread1));
        sleep(1);
        queue_running.pop();
        queue_running.push(pcb_obj);
        pthread_mutex_unlock((&mutex_locked_thread1));
        scheduler_ptr->send_running_queue_to_waiting_queue(pcb_obj);
    }
    // if the cpu time is greater than the time slice
    else if (pcb_obj.cpu_time > TIME_SLICE)
    {
        // then Run the Process for particular time slice and Send back to the Ready Queue
        // Here we have to execute the Process for the Given Time Slice..
        pcb_obj.cpu_time = pcb_obj.cpu_time - TIME_SLICE;
        pthread_mutex_lock((&mutex_locked_thread1));
        sleep(1);
        queue_running.pop();
        queue_running.push(pcb_obj);
        pthread_mutex_unlock((&mutex_locked_thread1));

        // now send this process to the Ready Queue again :)
        scheduler_ptr->send_running_queue_to_ready_queue(pcb_obj);
    }
    else
    {
        // it means that the Process will be executed in the time slice so terminate it lol
        scheduler_ptr->terminate_the_process_to_terminated_queue(pcb_obj);
    }
}

// Take Pcb object and Run the Process
void Processer::Algo_Premitive_Priority(PCB pcb_obj, Scheduler *scheduler_ptr)
{
    // Here we will Perform the Premitive Priority

    // send for the input output..
    if (pcb_obj.input_output_time > 0)
    {
        // send the process to the Waiting Queue ///
        TOTAL_CONTEXT_SWICTING++;
        // adding 2 cuz
        // -- CPU TIME + I/O time (Assume 1 sec)
        //     So,    1 + 1 = 2
        TOTAL_EXECUTION_TIME = TOTAL_EXECUTION_TIME + 2;
        // drcreasing the i/o by one because it' going for input
        pcb_obj.input_output_time = (pcb_obj.input_output_time) - 1;
        // just assuming it will go to i/o after 1 sec
        pcb_obj.cpu_time = (pcb_obj.cpu_time) - 1;
        // send the Process to the waiting Queue
        // queue_waiting.push(pcb_obj);
        pthread_mutex_lock((&mutex_locked_thread1));
        sleep(1);
        queue_running.pop();
        queue_running.push(pcb_obj);
        pthread_mutex_unlock((&mutex_locked_thread1));
        scheduler_ptr->send_running_queue_to_waiting_queue(pcb_obj);
    } // if the cpu time is there and the Process is not completed
    else if (pcb_obj.cpu_time <= 0)
    {
        // Send the Process backkkkkk to The Ready Queue...
        // I have set the TIME CLICE TO 1 -- for this process by default
        // Here we have to execute the Process for the Given Time Slice..
        pcb_obj.cpu_time = pcb_obj.cpu_time - TIME_SLICE;
        pthread_mutex_lock((&mutex_locked_thread1));
        sleep(1);
        queue_running.pop();
        queue_running.push(pcb_obj);
        pthread_mutex_unlock((&mutex_locked_thread1));

        // now send this process to the Ready Queue again :)
        scheduler_ptr->send_running_queue_to_ready_queue(pcb_obj);
    }
    else
    {
        scheduler_ptr->terminate_the_process_to_terminated_queue(pcb_obj);
    }
}
// Take Pcb object and Run the Process
void Processer::Algo_First_come_First_server(PCB pcb_obj, Scheduler *scheduler_ptr)
{

    // Here we have to Perform the FCFS
    //  cout << "\n Process " << pcb_obj.process_name << " has To be Executed in " << pcb_obj.cpu_time << " with " << pcb_obj.input_output_time << " I/O";

    // Here i will be counting the Total Execution time

    if (pcb_obj.input_output_time > 0)
    {
        // send the process to the Waiting Queue ///
        TOTAL_CONTEXT_SWICTING++;
        // adding 2 cuz
        // -- CPU TIME + I/O time (Assume 1 sec)
        //     So,    1 + 1 = 2
        TOTAL_EXECUTION_TIME = TOTAL_EXECUTION_TIME + 2;
        // drcreasing the i/o by one because it' going for input
        pcb_obj.input_output_time = (pcb_obj.input_output_time) - 1;
        // just assuming it will go to i/o after 1 sec
        pcb_obj.cpu_time = (pcb_obj.cpu_time) - 1;
        // send the Process to the waiting Queue
        // queue_waiting.push(pcb_obj);
        pthread_mutex_lock((&mutex_locked_thread1));
        sleep(1);
        queue_running.pop();
        queue_running.push(pcb_obj);
        pthread_mutex_unlock((&mutex_locked_thread1));
        scheduler_ptr->send_running_queue_to_waiting_queue(pcb_obj);
    }
    else
    {
        scheduler_ptr->terminate_the_process_to_terminated_queue(pcb_obj);
    }
}

//-----------------------Scheduler--------------------------------//

void Scheduler::send_running_queue_to_ready_queue(PCB pcb_obj)
{
    // Here the Thread will send the Running Process to the
    pthread_mutex_lock(&mutex_locked_thread1);
    // Now we have to Push to ready and remove from running with new time slices
    TOTAL_CONTEXT_SWICTING++;
    queue_ready.push(queue_running.front());
    queue_running.pop();
    pthread_mutex_unlock(&mutex_locked_thread1);
}
void Scheduler::terminate_the_process_to_terminated_queue(PCB pcb_obj)
{
    // get the process executed to the Terminated queue

    sleep(1);
    pthread_mutex_lock((&mutex_locked_thread1));
    queue_terminated.push(queue_running.front());
    queue_running.pop();
    pthread_mutex_unlock((&mutex_locked_thread1));
    // Now once it's terminated show and GOOOO
    TOTAL_EXECUTION_TIME = TOTAL_EXECUTION_TIME + pcb_obj.cpu_time;
    cout << "\n -----------COMPLETED EXECUTION -------------\n";
    cout << pcb_obj.process_name << "\n"
         << "-----------------------------------";
}
void Scheduler::send_running_queue_to_waiting_queue(PCB obj)
{
    TOTAL_CONTEXT_SWICTING++;
    // This function will send the running Queue to the waiting Queue
    PCB debug = queue_running.front();
    cout << "\n QUEUE Running " << debug.process_name << " " << debug.input_output_time << "   " << debug.cpu_time;
    pthread_mutex_lock((&mutex_locked_thread1));
    queue_waiting.push(queue_running.front());
    queue_running.pop();
    pthread_mutex_unlock((&mutex_locked_thread1));
}
void Scheduler::send_waiting_queue_to_ready_queue()
{
    // if waiting queue is empty then wait for something
    //  this will be a thread working in it's own zone
    TOTAL_TIME_READY_STATE++;
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
        // if waiting queue is not empty the do the following
        if (!queue_waiting.empty())
        {

            // cout << "\n RUNNING IN THE BACKGROUND FOR NO REASON";
            pthread_mutex_lock(&mutex_locked_thread1);
            TOTAL_CONTEXT_SWICTING++;
            queue_ready.push(queue_waiting.front());
            queue_waiting.pop();
            pthread_mutex_unlock(&mutex_locked_thread1);
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
    // this->Print_Output();
    //  This function will send the new Queue to Ready according to Arrival Time
    //  cout << "\n" << queue_new.front().process_name;

    while (!queue_new.empty())
    {
        cout << "\n NOTE: CPU is running with " << CPU_CORES << " Threads\n";
        cout << "\n--------READY QUEUE-----\n";
        cout << "\n Process " << queue_new.front().process_name << " Arrived \n";
        pthread_mutex_lock(&mutex_locked_thread1);
        queue_ready.push(queue_new.front());
        queue_new.pop();
        pthread_mutex_unlock(&mutex_locked_thread1);
        // Now once we get the Process into the Ready Queue Now it's time to execute it....
        cout << "\n-----------------------\n";
        sleep(queue_new.front().arrival_time);
    }
}
void Print_Output()
{
    // Shows the Output on the TERMINAL Every Few Seconds..
    //  this will be a thread working in it's own zone

    if (pthread_create(&kernel_print_output, NULL, helper_Print_Output, NULL) != 0)
    {
        perror("\n Kernel Threads are unable to create \n");
        exit(0);
    }
    pthread_detach(kernel_print_output);

    cout << "\n After THREAD CREATION \n";
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
    Print_Output();
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
        // sem_wait(&samphore_multithreading);
        sleep(1);
        // locked so one thread can write to the Queue at a time
        //    cout << "\nThread waiting: " << pthread_self() << "\n";
        while (queue_ready.empty())
            ;
        //   cout << " \nThread is Running : " << pthread_self() << "\n";
        // get the PCB and give it to the Processer
        PCB tmp_obj = queue_ready.front();
        queue_ready.pop();
        if (ALGO == 'f')
        {
            //   cout << "Running the FCFS";
            // GETTING TO THE RUNNING STATE
            TOTAL_CONTEXT_SWICTING++;
            queue_running.push(tmp_obj);
            my_cpu->Algo_First_come_First_server(tmp_obj, this);
        }
        else if (ALGO == 'r')
        {
            TOTAL_CONTEXT_SWICTING++;
            // Alright We got the Process in the ready Queue. Now we will send it to the Running Queue
            queue_running.push(tmp_obj);
            my_cpu->Algo_Round_Robin(tmp_obj, this);
        }
        else
        {
            // This is Preimitive Prority
            TOTAL_CONTEXT_SWICTING++;
            // We have to check the Prority of the Process before we send the Process to the Running Queue.

            // Iterate the Ready Queue for Largest Priority and send that to the Running Queue

            queue<PCB> tmp_queue = queue_ready;

            // Now we have to push everything in queue_Ready to the Prority Queue
            while (!tmp_queue.empty())
            {
                queue_ready_proprity.push(tmp_queue.front());
                tmp_queue.pop();
            }

            // Now we have a ready Queue where Priority Processes will always be at the top..

            // save the object
            tmp_obj = queue_ready_proprity.top();
            // send the top one to the Running state
            queue_running.push(queue_ready_proprity.top());
            queue_ready_proprity.pop();
            // Run the Algooo
            my_cpu->Algo_Premitive_Priority(tmp_obj, this);
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
    // ceating a thread
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
    // void Print_Output();
    // static void *helper_Print_Output(void *p);
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
        // just removing the Extra Spaces nothing fancy in here
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
    if (argc < 5 || argc > 6)
    {
        cout << "\n Please Use the Following Format \n";
        cout << "\nos-kernel <Input file> <# CPUs> r <timeslice> <Output file>\n";
        cout << "\nos-kernel <Input file> <# CPUs> f <Output file>\n";
        cout << "\nos-kernel <Input file> <# CPUs> p <Output file>\n";
    }
    else
    {
        //  cout << argv[3];
        // checking some argument..
        CPU_CORES = stoi(argv[2]);
        ALGO = *argv[3];
        sem_init(&samphore_multithreading, 0, CPU_CORES);
        if (!(*(argv[3]) == 'f' || *(argv[3]) == 'p' || *(argv[3]) == 'r'))
        {
            cout << "\n Invalid Aurgumnets \n";
            exit(1);
        }
        else
        {
            if (*argv[3] == 'r')
            {
                ALGO = *argv[3];
                TIME_SLICE = stoi(argv[4]);
                OUTPUT_FILE_NAME = argv[5];
            }
            else
            {
                // this is the Algo we are going to RUNnnn
                ALGO = *(argv[3]);
                OUTPUT_FILE_NAME = argv[4];
            }
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
                    // Print_Output();
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

    sem_destroy(&samphore_multithreading);
    pthread_exit(0);
    return 0;
}