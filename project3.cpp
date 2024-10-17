#include <iostream>
#include <string>

using namespace std;

/*
fixed issue with promotion, but now there is an issue
with the output, after reorder is called it queues all the 
operations back and causes all the output statment to occur

I'm about to implement new code to try to adjust this but am commiting incase it blows up

The trash file is going to contain the old version as a backup
*/

// CPUJob class definition
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

// Queue class template
template <class DT>
class Queue {
public:
    DT* JobPointer;
    Queue<DT>* next;

    Queue(DT* job) : JobPointer(job), next(nullptr) {}
};

// NovelQueue class template (without vectors, using static array instead)
template <class DT>
class NovelQueue {
public:
    Queue<DT>* front;        // Pointer to the front of the queue
    Queue<DT>** NodePtrs;    // Array of pointers to Queue nodes
    int size;                // Number of elements in the queue

    NovelQueue() : front(nullptr), size(0) {
        NodePtrs = new Queue<DT>*[100];  // Assuming a max of 100 jobs
    }

    // Enqueue a new job
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
            output += "Enqueued Job: ";
            output += "\n";
            job->display(output);
            output += "Jobs after enqueue:\n";
            display(output);
        }
    }

    // Dequeue a job
    DT* dequeue(string& output) {
        if (!front) return nullptr;

        Queue<DT>* temp = front;
        front = front->next;

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
        output += "Jobs after dequeue:\n";
        display(output);
        return job;
    }

    // Modify job attributes
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

    // Change a specific field of a job
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

    // Promote a job within the queue
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


    // Reorder the queue based on a specified attribute and return a new queue
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
                        break;
                    case 3:  // Sort by CPU time consumed
                        if (tempQueue[i]->cpu_time_consumed > tempQueue[j]->cpu_time_consumed) swap = true;
                        break;
                    case 4:  // Sort by memory consumed
                        if (tempQueue[i]->memory_consumed > tempQueue[j]->memory_consumed) swap = true;
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

    // List jobs sorted by job_id
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

    // Display all jobs
    void display(string& output) const {
        for (int i = 0; i < size; ++i) {
            NodePtrs[i]->JobPointer->display(output);
        }
    }

    // Count the number of jobs
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




