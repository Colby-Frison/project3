#include <iostream>
#include <string>

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * Colby Frison* * * * * * * * * * * * * * * * * * *
 * * * * * * * Project 3 - Data structures || C S - 2413 * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

This project is essentially creating a queueu data structure in order to organize
The operations of a CPU. The main few classes are : CPUJob, Queue, and NovelQueue.

More information will be provided in the header of each class, but I'll give a 
general breakdown here.

Starting with CPUJob it serves the fuction of simply storing the information of 
the individual jobs, so its like the _info of the element.It holds info as well
as displaying the job.

Next up is queue class, its littally just a queue data structure quite simple.

Last is NovelQueue, this is where most of the heavy lifting is done. It has a 
number of methods that I'll go into down there. But all in all this is where most
of the functions used in the main method come from.
________________________________________________________________________________

Part 2 LLM usage : 559

Part 3 : 
________________________________________________________________________________

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
The CPUJob class is pretty bare bones, it stores all of the info of each operation
Which is usually refered to as attributes. Each attribute is kind of arbitrary as 
they dont actually impact the operation beyond beying used as a means of sorting.

The constructor is simple. 

The two display methods seem strange and they lowkey are. The way I handle the output 
is through a output string which is added to throughtout the course of the program, 
which is why the first display has an output varible passed in. The next one simply 
outputs to console, this was primarily used for debugging (mainly with the promote 
and reorder methods).
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class CPUJob {
public:
    int job_id;
    int priority;
    int job_type;
    int cpu_time_consumed;
    int memory_consumed;

    // Constructor
    CPUJob(int id, int pr, int type, int cpu_time, int memory)
        : job_id(id), priority(pr), job_type(type), 
          cpu_time_consumed(cpu_time), memory_consumed(memory) {}

    // Display method
    void display(string& output) const {
        output += "Job ID: " + to_string(job_id) + ", Priority: " + to_string(priority) +
                  ", Job Type: " + to_string(job_type) + ", CPU Time Consumed: " + 
                  to_string(cpu_time_consumed) + ", Memory Consumed: " + 
                  to_string(memory_consumed) + "\n";
    }

    //display for debugg purposes
    void display() const {
        cout << "Job ID: " << to_string(job_id) << ", Priority: " << to_string(priority) << 
                  ", Job Type: " << to_string(job_type) << ", CPU Time Consumed: " << 
                  to_string(cpu_time_consumed) << ", Memory Consumed: " << 
                  to_string(memory_consumed) << endl;
    }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
This is litterally just a queue, jobPointer is the variable storing the job (just 
like _info stores the info). And next points to the next node in the queue
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template <class DT>
class Queue {
public:
    DT* JobPointer;
    Queue<DT>* next;

    Queue(DT* job) : JobPointer(job), next(nullptr) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
                         Okay this is gonna be alot.

First of all the class varaibles. front is simply a pointer that points to the 
front of the queue which is used to keep track of whats next, and is used to increment
the queue. NodePtrs, as an array of pointers that points to each element is the 
queue. This allows for an effiecient traversal of the queue, during operations like
promote change and modify, where a specific element not at the front of the queue 
needs to be accessed. Lastly size simply shows how many elements are in the queue.

For the method decleration I'll do that at the begining of each so I dont have to 
keep going back and forth.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template <class DT>
class NovelQueue {
public:
    Queue<DT>* front;        // Pointer to the front of the queue
    Queue<DT>** NodePtrs;    // Array of pointers to Queue nodes
    int size;                // Number of elements in the queue

    /* 
    default constructor, simply initilizes empty. Then for nodePtrs array it defaults
    to size of 100, then there is a resize if nessesary later
    */
    NovelQueue() : front(nullptr), size(0) {
        NodePtrs = new Queue<DT>*[100];  // Assuming a max of 100 jobs
    }

    /* 
    enqueue is pretty simple, it checks to make sure the operation isnt already queued.
    Then if not it creates a new node with 'job', if front is null it is made front, 
    if not it increments the queue, once the next is nullptr it stops incremeneting
    and newNode is made next. It then adds the node to the array, by putting it in at index 
    size + 1. 

    Handling outputs isnt complicated, but its a little more complicated than other methods.
    Methods like reorder, are implemeneted by creating a temp queue and enqueing the elments 
    in the new order, this creates an issue as it prints stuff that shouldnt be. this is handled
    with the boolean. Besides that it just displays the specific job by the CPUJob display,
    and prints all the jobs with the NovelQueues display
    */
    void enqueue(DT* job, string& output, bool print_output = true) {
        // Check if job ID already exists
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->JobPointer->job_id == job->job_id) {
                output += "Job ID " + to_string(job->job_id) + " already exists!\n";
                return;  // Stop adding the job
            }
        }


        Queue<DT>* newNode = new Queue<DT>(job);
        if (!front) {
            front = newNode;
        } else {
            Queue<DT>* temp = front;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        NodePtrs[size++] = newNode;

        if (print_output) {
            output += "Enqueued Job:";
            output += " ";
            output += "\n";
            job->display(output);
            output += "Jobs after enqueue: ";
            output += "\n";
            display(output);
        }
    }

    /*
    Dequeue also pretty simple, if front is empty, return nothing as there is nothing
    to dequeue. then a temp for front is made, front is set to next. Then the array 
    is incremeneted to make the ajustment. Finally, size is decremeneted, and temp is 
    deletd. Also the print is made which is the same as enqueue basically just minus 
    the boolean thing. Also it returns job as it is deleted in the main method for some 
    reason.
    */
    DT* dequeue(string& output) {
        if (!front) return nullptr;

        Queue<DT>* temp = front;
        front = front->next;

        //I'm 99% sure I had this here for debugging 
        //but I'm scared to delete it
        DT* job = temp->JobPointer;

        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i] == temp) {
                for (int j = i; j < size - 1; ++j) {
                    NodePtrs[j] = NodePtrs[j + 1];
                }
                break;
            }
        }

        size--;
        delete temp;

        output += "Dequeued Job: ";
        output += "\n";
        job->display(output);
        output += "Jobs after dequeue: ";
        output += "\n";
        display(output);
        return job;
    }

    /* 
    Modify is probably the simpelst one, it just changes a certain job based on what info was passed in
    job is found by iterating array. Once job is found 'job_found' is set to true then the stuff is chaged.

    'job_found' is important as if a job is called that isnt there it wont let the user know. So thats why
    its there.
    */
    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_memory, string& output) {
        bool job_found = false;
        
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->JobPointer->job_id == job_id) {
                job_found = true;
                NodePtrs[i]->JobPointer->priority = new_priority;
                NodePtrs[i]->JobPointer->job_type = new_job_type;
                NodePtrs[i]->JobPointer->cpu_time_consumed = new_cpu_time;
                NodePtrs[i]->JobPointer->memory_consumed = new_memory;

                output += "Modified Job ID " + to_string(job_id) + ":\n";
                NodePtrs[i]->JobPointer->display(output);
                output += "Jobs after modification:\n";
                display(output);

                break;
            }
        }

        if (!job_found) {
            output += "Job with ID " + to_string(job_id) + " not found in the queue.\n";
        }
    }

    /*
    same as modify, but just for a specific attribute
    */
    void change(int job_id, int field_index, int new_value, string& output) {
        bool job_found = false;

        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->JobPointer->job_id == job_id) {
                job_found = true;

                switch (field_index) {
                    case 1: NodePtrs[i]->JobPointer->priority = new_value; break;
                    case 2: NodePtrs[i]->JobPointer->job_type = new_value; break;
                    case 3: NodePtrs[i]->JobPointer->cpu_time_consumed = new_value; break;
                    case 4: NodePtrs[i]->JobPointer->memory_consumed = new_value; break;
                    default: break;
                }

                output += "Changed Job ID " + to_string(job_id) + " field " + to_string(field_index) + " to " + to_string(new_value) + ":\n";
                NodePtrs[i]->JobPointer->display(output);
                output += "Jobs after changing field:\n";
                display(output);
                
                break;
            }
        }

        if (!job_found) {
            output += "Job with ID " + to_string(job_id) + " not found in the queue.\n";
        }
    }

    /*
    Promote simply promotes a certain job in the queue by a certaain amount of position.
    If it tries to go beyond 0 it is made 0.

    I was struggling with this one so steps are put thorughout the method for simplicity
    */
    void promote(int job_id, int positions, string& output) {
        bool job_found = false;

        Queue<DT>* prev = nullptr;
        Queue<DT>* curr = front;

        // Step 1: Find the job in the linked list
        while (curr && curr->JobPointer->job_id != job_id) {
            prev = curr;
            curr = curr->next;
        }

        // If the job was found
        if (curr) {
            job_found = true;
            // Step 2: Remove the job from the current position in the linked list
            if (prev) {
                prev->next = curr->next;  // Unlink from the current position
            } else {
                front = curr->next;  // Update front if the job is at the front
            }

            curr->next = nullptr;  // Temporarily set the next pointer to nullptr

            // Step 3: Update NodePtrs to reflect the removal of the job
            int current_index = -1;
            for (int i = 0; i < size; ++i) {
                if (NodePtrs[i] == curr) {
                    current_index = i;
                    break;
                }
            }

            // Shift NodePtrs to remove the current job
            for (int i = current_index; i < size - 1; ++i) {
                NodePtrs[i] = NodePtrs[i + 1];
            }
            NodePtrs[size - 1] = nullptr;
            size--;

            // Step 4: Calculate the new position after promotion
            int new_pos = current_index - positions;
            if (new_pos < 0) new_pos = 0;  // Prevent out-of-bounds

            // Step 5: Insert the job in the new position in the linked list
            Queue<DT>* insert_prev = nullptr;
            Queue<DT>* insert_pos = front;
            for (int i = 0; i < new_pos && insert_pos != nullptr; ++i) {
                insert_prev = insert_pos;
                insert_pos = insert_pos->next;
            }

            if (insert_prev) {
                insert_prev->next = curr;  // Insert after insert_prev
            } else {
                front = curr;  // Insert at the front
            }
            curr->next = insert_pos;  // Connect to the next job in the list

            // Step 6: Update NodePtrs to insert the job in the correct position
            for (int i = size; i > new_pos; --i) {
                NodePtrs[i] = NodePtrs[i - 1];
            }
            NodePtrs[new_pos] = curr;
            size++;

            // Step 7: Output the result
            output += "Promoted Job ID " + to_string(job_id) + " by " + to_string(positions) + " Position(s):\n";
            curr->JobPointer->display(output);
            output += "Jobs after promotion:\n";
            display(output);
        }

        if (!job_found) {
            output += "Job with ID " + to_string(job_id) + " not found in the queue.\n";
        }
    }


    /*
    Queue is reordered based on a specfic attribute. This is done by swapping the 
    elements in a tmeporary array, then enqueueing those elements into a queue and
    setting it to the og queue, which is why the output varibale is important for the
    enqueue method.
    */
    NovelQueue<DT>* reorder(int attribute_index, string& output) {
        NovelQueue<DT>* newQueue = new NovelQueue<DT>();

        // Copy the NodePtrs to a temp array for sorting
        DT* tempQueue[100];
        for (int i = 0; i < size; ++i) {
            tempQueue[i] = NodePtrs[i]->JobPointer;
        }

        // Sort the array based on the attribute
        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
                bool swap = false;

                switch (attribute_index) {
                    case 1:  // Sort by job_id
                        if (tempQueue[i]->job_id > tempQueue[j]->job_id) swap = true;
                        break;
                    case 2:  // Sort by priority
                        if (tempQueue[i]->priority > tempQueue[j]->priority) swap = true;
                        else if (tempQueue[i]->priority == tempQueue[j]->priority && tempQueue[i]->job_id > tempQueue[j]->job_id)
                            swap = true;  // Fallback to job_id if priority is the same
                        break;
                    case 3:  // Sort by CPU time consumed
                        if (tempQueue[i]->cpu_time_consumed > tempQueue[j]->cpu_time_consumed) swap = true;
                        else if (tempQueue[i]->cpu_time_consumed == tempQueue[j]->cpu_time_consumed && tempQueue[i]->job_id > tempQueue[j]->job_id)
                            swap = true;
                        break;
                    case 4:  // Sort by memory consumed
                        if (tempQueue[i]->memory_consumed > tempQueue[j]->memory_consumed) swap = true;
                        else if (tempQueue[i]->memory_consumed == tempQueue[j]->memory_consumed && tempQueue[i]->job_id > tempQueue[j]->job_id)
                            swap = true;
                        break;
                    default:
                        output += "Invalid attribute index!\n";
                        return newQueue;
                }

                if (swap) {
                    DT* temp = tempQueue[i];
                    tempQueue[i] = tempQueue[j];
                    tempQueue[j] = temp;
                }
            }
        }

        // Enqueue the sorted jobs into the new queue
        for (int i = 0; i < size; ++i) {
            newQueue->enqueue(tempQueue[i], output, false);  // Suppress output
        }

        output += "Reordered Queue by attribute " + to_string(attribute_index) + ":\n";
        newQueue->display(output);

        return newQueue;
    }

    /*
    List jobs prints the list of jobs in order of job_id, but without saving them in that pos.
    It's pretty simple though, it creates a temp queue array, copies elements from nodePtr, then 
    sorts that array. From there it loops through temp and outputs the job.
    */
    void listJobs(string& output) const {
        // Create a temporary array to hold the jobs in sorted order
        Queue<DT>* tempQueue[100];

        // Copy the current NodePtrs into the temp array
        for (int i = 0; i < size; ++i) {
            tempQueue[i] = NodePtrs[i];
        }

        // Sort the temp array by job_id
        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (tempQueue[i]->JobPointer->job_id > tempQueue[j]->JobPointer->job_id) {
                    // Swap jobs if the current job_id is greater than the next
                    Queue<DT>* temp = tempQueue[i];
                    tempQueue[i] = tempQueue[j];
                    tempQueue[j] = temp;
                }
            }
        }

        // Output the sorted jobs
        output += "List of jobs sorted by job IDs:\n";
        for (int i = 0; i < size; ++i) {
            tempQueue[i]->JobPointer->display(output);
        }
    }

    /*
    Display for NovelQueue was nessesary as it needs to go thorught the entire list, so it was 
    easier to just have a method to do it on its own implementing CPUJob's display. This is the method 
    used in most of the methods for handling output
    */
    void display(string& output) const {
        for (int i = 0; i < size; ++i) {
            NodePtrs[i]->JobPointer->display(output);
        }
    }

    // Count the number of jobs
    //kinda silly but oh well
    int count() const {
        return size;
    }

    // Destructor to clean up dynamically allocated memory
    ~NovelQueue() {
        delete[] NodePtrs;
        while (front) {
            Queue<DT>* temp = front;
            front = front->next;
            delete temp;
        }
    }
};


// Main function
/*
Reads through all commands and uses a switch case to handle each chip. At the end
it print is the output String, and deletes remaininf memory.
*/
int main() {
    int n;  // Number of commands
    cin >> n;  // Read the number of commands
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();  // Fix instantiation
    char command;

    string output = "";

    // Variables for job attributes
    int job_id, priority, job_type, cpu_time, memory;
    int new_priority, new_job_type, new_cpu_time, new_memory, field_index, new_value;
    int positions, attribute_index;

    for (int i = 0; i < n; ++i) {
        cin >> command;
        switch (command) {
            case 'A': {  // Add (Enqueue)
                cin >> job_id >> priority >> job_type >> cpu_time >> memory;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time, memory);
                myNovelQueue->enqueue(newJob, output);  // Works with CPUJob* directly
                break;
            }
            case 'R': {  // Remove (Dequeue)
                CPUJob* removedJob = myNovelQueue->dequeue(output);  // Returns CPUJob*
                if (removedJob) {
                    delete removedJob;
                }
                break;
            }
            case 'M': {  // Modify
                cin >> job_id >> new_priority >> new_job_type >> new_cpu_time >> new_memory;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time, new_memory, output);
                break;
            }
            case 'C': {  // Change Job Values
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value, output);
                break;
            }
            case 'P': {  // Promote
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions, output);
                break;
            }
            case 'O': {  // Reorder
                cin >> attribute_index;
                NovelQueue<CPUJob>* reorderedQueue = myNovelQueue->reorder(attribute_index, output);
                myNovelQueue = reorderedQueue;
                break;
            }
            case 'D': {  // Display
                output += "Displaying all jobs in the queue:\n";
                myNovelQueue->display(output);
                break;
            }
            case 'N': {  // Count
                output += "Number of elements in the queue: " + to_string(myNovelQueue->count()) + "\n";
                break;
            }
            case 'L': {  // List Jobs
                myNovelQueue->listJobs(output);
                break;
            }
            default:
                output += "Invalid command!\n";
        }
    }

    cout << output;
    delete myNovelQueue;
    
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * * * * * * * * * * * * * Part 2 LLM usage* * * * * * * * * * * * * * * * * *

Primarily the LLM was used to debug in the later stages of development when there 
was a lot of smaller output errors, or simple logic errors. I also used the LLM a 
before even starting to code to explain the project a little more extensvley. Mainly
just to like summarize and simply or even expand upon certain areas, as some language 
was either vague, or it just didnt make sense (to me). 

I also used it to gain a deeper understanding of some certain aspects of c++ or 
data structures being used.

So below I'm going to include the important chat logs, but before that some explanation
on my methodology. 

I started with giving it the pdf to gain a general idea of the project, I then gave
it notes from class about the project so it had some more info to work off of. 
From that it gave a general premis eof the project along with important methods,
and key datastructures, which wasnt super useful, but considering how LLMs work it
is an important step.

I then gave it what I had developed so far which contained most of the methods being 
complteley done, with only small errors remaining in various section. I then used 
it to debug the code by litterally copying and pasting it. From that I generated an
output, which of course was wrong, I gave it what it produced with its respective input
along with what the desired result is. It then changed some small stuff, mainly
orderering of outputs, along with the checker for enqueue. 

I tried a whole tangent thing where I had a method that sorted elements based on an
attribute index, it worked if I remember correctly, but it was silly, and too complex
for what it was ment for. So I went back to the og implementation. 

I think the last thing I used it for was fixing the output, I had an issue with sorting
which I though was because of white space, but Im actually slow, and the issue was 
when reorder was called, if two jobs had the same value for one of their attributes
they would be queued back in the order they were, but I found the desired way to handle 
this was by then defaulting to the job_id to find priority(this is how it was done in
the exmaple output). It took me forever to implement this and I feel stupid now.

so here are the chats:
_________________________________________________________________________________

Me : 

Here is my code :

//code//

along with the input and desired output: 

//input and output//

here is my output : 

//output//

there is an issue in this section of output

Jobs after promotion:
Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096
Job ID: 101, Priority: 5, Job Type: 2, CPU Time Consumed: 150, Memory Consumed: 2000
Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
Job ID: 104, Priority: 2, Job Type: 3, CPU Time Consumed: 300, Memory Consumed: 2048
Job ID: 105, Priority: 6, Job Type: 5, CPU Time Consumed: 200, Memory Consumed: 512
Dequeued Job:
Job ID: 101, Priority: 5, Job Type: 2, CPU Time Consumed: 150, Memory Consumed: 2000

you can see that 103 is first in the queue, but when dequeue is called 101 is deququed 
instead, please help in fixing this issue
________________________________________________________________________________

Chat gpt :

The issue you're encountering happens because the promotion logic only affects the 
array of node pointers (NodePtrs), but it does not update the actual linked list 
order. When dequeue is called, the linked list's front is used to determine which 
job is dequeued, and this list is not affected by the promotion method.

To resolve this, you need to ensure that both the array (NodePtrs) and the linked 
list are updated during the promote operation. Here's an adjusted version of the 
promote method that updates both the array and the linked list:

//code that actaully worked (shocking)//

Key Adjustments:

    Removal from Both Linked List and Array: The job is fully removed from both the 
    linked list and the NodePtrs array before reinserting it.
    Avoiding Duplication: The method ensures that no duplicates of the job are 
    left behind by correctly shifting the array elements after removing the job.
    Correct Positioning: The job is reinserted in both the linked list and the 
    array at the new position based on the promotion logic.

Test This:

Try running this version of the promote method and check the output. It should 
now correctly update the linked list and NodePtrs array without duplicating jobs.
________________________________________________________________________________

me : 

Very good job that works perfectly.

Okay now hopefully the last change is with the 'D' or display chip which display 
everything in the queue. The first time it works properly, but the second time it 
isnt sorted by job id, as the reorder chip had just been called reordering by 
priority. So make a change to display to function properly.

wrong:
Displaying all jobs in the queue:
Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
Job ID: 104, Priority: 2, Job Type: 3, CPU Time Consumed: 300, Memory Consumed: 2048
Job ID: 106, Priority: 4, Job Type: 2, CPU Time Consumed: 320, Memory Consumed: 5120
Job ID: 105, Priority: 7, Job Type: 5, CPU Time Consumed: 220, Memory Consumed: 1024
Job ID: 107, Priority: 3, Job Type: 4, CPU Time Consumed: 180, Memory Consumed: 1500

right:
Displaying all jobs in the queue:
Job ID: 104, Priority: 2, Job Type: 3, CPU Time Consumed: 300, Memory Consumed: 2048
Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
Job ID: 107, Priority: 3, Job Type: 4, CPU Time Consumed: 180, Memory Consumed: 1500
Job ID: 106, Priority: 4, Job Type: 2, CPU Time Consumed: 320, Memory Consumed: 5120
Job ID: 105, Priority: 7, Job Type: 5, CPU Time Consumed: 220, Memory Consumed: 1024
________________________________________________________________________________

chat gpt : 

To fix the display method so that it always displays the jobs sorted by job_id, 
even after a reordering by a different attribute (like priority), you can modify 
the display method to sort the jobs by job_id before printing them.

Updated display Method:

//code (I stop it mid way through as it doing the wrong thing)//
________________________________________________________________________________

me : 

oh you have it mixed up the 'L' chip make sit print in order of job_id. The 'D' 
chip displays in order of the queue
________________________________________________________________________________

chat gpt :

Got it! Since the D command displays jobs in the order they appear in the queue, 
and you've just reordered by priority, the current behavior is actually correct. 
The jobs are displayed in the queue's current order, reflecting the most recent 
reordering, whether by priority or another attribute.

However, if the requirement is to always display jobs in their current queue 
order (which changes based on sorting or promotion), the display method is 
functioning properly. Here's a reminder of what happens:

    After reordering by priority (or any other attribute), jobs are displayed 
    in that order, not by job ID. The L command lists jobs sorted specifically 
    by job ID.

So, no changes are needed unless you want to adjust how reordering works. The 
current implementation already reflects the correct behavior for the D command, 
displaying jobs in their current queue order. Let me know if any further clarification 
is needed!
________________________________________________________________________________

me: 

then the issue must be when the queue is reordered it isnt saved properly
________________________________________________________________________________

chat gpt : 

Let's fix the listJobs method to correctly sort jobs by job_id before displaying 
them. The method needs to temporarily sort the jobs by job_id and then print them 
in the correct order, without affecting the actual queue or NodePtrs array.

//working code//

Key Changes:

    Temporary Array: A temporary array (tempQueue) is created to store the current jobs. 
    This array is used to sort the jobs by job_id without affecting the actual queue structure.
    Sorting by job_id: The array is sorted in ascending order of job_id using a simple 
    bubble sort algorithm. Output Sorted Jobs: After sorting, the jobs are displayed 
    in the correct order based on job_id.

Explanation:

    No Changes to the Actual Queue: The actual queue and NodePtrs are not modified. 
    This method only affects the temporary array used for listing. Sorting by Job 
    ID: Jobs are sorted by job_id and then displayed in that order.

Try this updated listJobs method, and it should now correctly sort jobs by job_id 
as required. Let me know if you need further adjustments!
________________________________________________________________________________

I then go on an entire tangent trying to fix a white space issue that 
doesnt actually exist.

No problems are solved no ideas are implemeneted. Honestly a waste of time
________________________________________________________________________________

me :

Im just gonna ignore it for now, but I've encountered a few other issues, if the 
user tries to access a job_id that no longer exists it need to print out a message. 
Also if a job_id already exxists another operation of the same job_id cannot be 
added, add these exceptions to any methods that may require them.
________________________________________________________________________________

chat gpt :

To handle these two scenarios (checking for non-existent job IDs and preventing 
duplicate job additions), we can add checks to the relevant methods (enqueue, modify, 
change, etc.). Here’s how we can incorporate these changes:
1. Prevent Duplicate Job IDs on enqueue:

We need to check if the job ID already exists before adding a new job. If it does, 
we can print an error message.
2. Handle Non-existent Job IDs on Access:

For methods like modify, change, promote, etc., we should print a message if the job ID is not found.
Updated Methods:

//updated enqueue modify change and promote methods //

Summary of Changes:

    Enqueue: If a job with the same job_id already exists, the program prints an 
    error message and doesn't add the new job.
    Modify, Change, Promote: If the requested job_id doesn't exist, an error message is printed.

These changes ensure that operations behave properly when handling job IDs. Let 
me know if this works for you!
________________________________________________________________________________

me : 

okay actaully last thing. When reordering if the attribute has a repeat element 
then go back to job_id to determine which should go first, make this change in 
the reorder method please.
________________________________________________________________________________

chat gpt : 

To implement the fallback to sorting by job_id when the attribute has repeat elements 
during reordering, we need to modify the reorder method. Specifically, when two jobs 
have the same value for the primary attribute being sorted, we will compare their 
job_id to decide the order.

Here's the updated reorder method that includes this fallback logic:
Updated reorder Method:

//code//

Key Changes:

    Fallback to job_id: When two jobs have the same value for the primary sorting attribute 
    (like priority, CPU time, or memory consumed), the jobs are then compared by job_id. 
    This ensures that jobs with the same attribute value are ordered by their job IDs.
    Preservation of Current Queue Structure: The method still clears the current queue 
    and then rebuilds it in the new order, maintaining the updates after sorting.

Example:

If you are sorting by priority and two jobs have the same priority, the job with 
the lower job_id will come first in the queue.

This should now work as expected, ensuring that when there are ties in the primary 
attribute being sorted, jobs are ordered by their job_id. Let me know if this works for you!
________________________________________________________________________________

me :

Thank you for help

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* * * * * * * * * * Part 3 Debugging and Testing Plan * * * * * * * * * * * * *

Most if not all of my testing was done with input 1 and output 1. Since it was so 
short I was able to visualy deduce issues and where the issue stems from. With 
this I was able to find what the porblems in why methods were what they stemmed 
from and how to fix it. But this of course had its limitations, so that when I made
the second display method in the CPUJob class, I used this to debug as inputs were 
read. This was really useful when I encountered issues where the data was stored 
in one medium, but no the other. I was able to use in line debugging to resolve these
issues.

One issue I encountered was in promote it was shown the queue was in the right location, 
but in the following dequeue it selected the wrong thing. I had similar issues with
the reorder method. To resolve this I just made sure the change was made ot both mediums
which resolved this pretty eaily, I felt kinda silly.

The other major issue I had was with the failsafes needed in the second and third 
inputs, I didnt encounter these until much later on since I was doing most testing 
with input 1. This was pretty simple just put in a chcker bool then have an if 
statement for checking.

The last major issue I can remember is when reordering based on an atrribute that 
isn't job id. Previously it would just sort it and if the attribute was the same
they would just be put back in the oredr they were recieved. The desired output
was to fall back to the job_id to sort. This was a simple fix, just an if statement
if same go to case 0. I was able to visually determine this issue with my output
vs. desired output.

Another method I found that was really useful in debugging was using git. 
I would push the desired output in and output.txt, I would then paste what my code 
output into the txt file, this would show me what lines were different. This allowed
me to very easily detect differences, and where the change was in the output. This was 
a very nice feature I used during debugging.

With that I think thats all the major notable issues I had, then again most coding
was done sleep deprived, so I probably missed something.

Thanks for grading, I (and the rest of the class) appreciate you :)

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */