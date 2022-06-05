#include <iostream>
#include <queue>
using namespace std;
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

     bool operator<(const PCB& rhs) const
    {
        return priority < rhs.priority;
    }

};
void showpq(priority_queue<PCB> gq)
{
    PCB test;
    priority_queue<PCB> g = gq;
    while (!g.empty()) {
        test = g.top();
        g.pop();
        cout <<"\n" << test.priority << "\t" << test.process_name << "\t" << "\n";
        
    }
    cout << '\n';
}
  
// Driver Code
int main()
{
    priority_queue<PCB> gquiz;
    PCB obj1;
    obj1.process_name ="lowest";
    obj1.priority=5;

    PCB obj2;
    obj2.process_name ="largest";
    obj2.priority=15;

    PCB obj3;
    obj3.process_name ="middle";
    obj3.priority=13;

    gquiz.push(obj3);
    gquiz.push(obj3);
    gquiz.push(obj3);
    gquiz.push(obj3);
    gquiz.push(obj1);
    gquiz.push(obj2);
    gquiz.push(obj3);
    gquiz.push(obj1);
  
  
    cout << "The priority queue gquiz is : ";
    showpq(gquiz);
  
    return 0;
}