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
                    cout << "\ntest " << index << "\n";
                    // first col
                    this->process_name = (process_info);
                }
                else if (index == 1)
                {
                     cout << "\ntest " << index << "\n";
                    // second col
                    this->priority = stod(process_info);
                }
                else if (index == 2)
                {
                     cout << "\ntest " << index << "\n";
                    // third col
                    this->arrival_time = stod(process_info);
                }
                else if (index == 3)
                {
                     cout << "\ntest " << index << "\n";
                    this->process_type = process_info;
                }
                else if (index == 4)
                {
                     cout << "\ntest " << index << "\n";
                    this->cpu_time = stod(process_info);
                }
                else if (index == 5)
                {
                     cout << "\ntest " << index << "\n";
                    this->input_output_time = stod(process_info);
                }
                else
                {
                     cout << "\ntest " << index << "\n";
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
             << this->priority ;
        cout << "\n"
             << this->arrival_time ;
        cout << "\n"
             << this->process_type ;
        cout << "\n"
             << this->cpu_time ;
        cout << "\n"
             << this->input_output_time ;
    }
};

class Kernel
{
    // a pointer that can access the PCB anytime
    PCB *pcb_arr;
    int count_pcb_entry;

public:
Kernel(){
    pcb_arr = new PCB[8];
    count_pcb_entry=0;
}
    void Implement_start(string file_name);
};

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
                //creaing a pcb for each process
                cout << "\n Counting " << count_pcb_entry;
               pcb_arr[count_pcb_entry].assign_values_to_pcb_variable(tp);
                count_pcb_entry++;
            }
            i++;
        }
        newfile.close(); // close the file object.
        cout << "\n Iusses\n";
        // reading the PCB values for testing.
        for (int i = 0; i < count_pcb_entry; i++)
        {
           pcb_arr[i].display_pcb_values();
        }
        
        
    }
}