#include <iostream>
#include <string>

using namespace std;

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
    void display() const {
        cout << "Job ID: " << job_id << " Priority: " << priority
             << " Job Type: " << job_type << " CPU Time Consumed: " 
             << cpu_time_consumed << " Memory Consumed: " << memory_consumed << endl;
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
private:
    Queue<DT>* front;
    Queue<DT>* rear;
    DT* NodePtrs[100];  // Static array for holding job pointers directly
    int size;

    // Helper function to sort NodePtrs based on job ID
    void sortNodePtrs() {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (NodePtrs[i]->job_id > NodePtrs[j]->job_id) {
                    DT* temp = NodePtrs[i];
                    NodePtrs[i] = NodePtrs[j];
                    NodePtrs[j] = temp;
                }
            }
        }
    }

public:
    NovelQueue() : front(nullptr), rear(nullptr), size(0) {}

    // Method to enqueue a new job
    void enqueue(DT* job) {  // Accepts CPUJob* directly
        Queue<DT>* newNode = new Queue<DT>(job);
        if (rear) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
        NodePtrs[size++] = job;  // Store the job pointer directly in the array
        sortNodePtrs();
    }

    // Method to dequeue a job
    DT* dequeue() {  // Returns CPUJob*
        if (!front) return nullptr;
        Queue<DT>* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        DT* job = temp->JobPointer;

        // Remove the dequeued job from NodePtrs
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i] == job) {
                for (int j = i; j < size - 1; ++j) {
                    NodePtrs[j] = NodePtrs[j + 1];
                }
                break;
            }
        }
        size--;
        delete temp;
        return job;
    }

    // Method to modify a job's attributes
    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_memory) {
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->job_id == job_id) {
                NodePtrs[i]->priority = new_priority;
                NodePtrs[i]->job_type = new_job_type;
                NodePtrs[i]->cpu_time_consumed = new_cpu_time;
                NodePtrs[i]->memory_consumed = new_memory;
                sortNodePtrs();
                break;
            }
        }
    }

    // Method to change a specific field of a job
    void change(int job_id, int field_index, int new_value) {
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->job_id == job_id) {
                switch (field_index) {
                    case 1: NodePtrs[i]->priority = new_value; break;
                    case 2: NodePtrs[i]->job_type = new_value; break;
                    case 3: NodePtrs[i]->cpu_time_consumed = new_value; break;
                    case 4: NodePtrs[i]->memory_consumed = new_value; break;
                    default: break;
                }
                sortNodePtrs();
                return;
            }
        }
    }

    // Method to promote a job closer to the front of the queue
    void promote(int job_id, int positions) {
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->job_id == job_id) {
                int new_pos = (i - positions < 0) ? 0 : i - positions;
                DT* temp = NodePtrs[i];
                for (int j = i; j > new_pos; --j) {
                    NodePtrs[j] = NodePtrs[j - 1];
                }
                NodePtrs[new_pos] = temp;
                break;
            }
        }
    }

    // Method to reorder based on a specified attribute
    NovelQueue<DT>* reorder(int attribute_index) {
        NovelQueue<DT>* newQueue = new NovelQueue<DT>();

        // Copy current jobs to a temporary array
        DT* tempQueue[100];
        for (int i = 0; i < size; ++i) {
            tempQueue[i] = NodePtrs[i];
        }

        // Sort based on the selected attribute
        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
                bool swap = false;
                if (attribute_index == 1 && tempQueue[i]->job_id > tempQueue[j]->job_id) {
                    swap = true;
                } else if (attribute_index == 2 && tempQueue[i]->priority < tempQueue[j]->priority) {
                    swap = true;
                } else if (attribute_index == 3 && tempQueue[i]->cpu_time_consumed < tempQueue[j]->cpu_time_consumed) {
                    swap = true;
                } else if (attribute_index == 4 && tempQueue[i]->memory_consumed < tempQueue[j]->memory_consumed) {
                    swap = true;
                }
                if (swap) {
                    DT* temp = tempQueue[i];
                    tempQueue[i] = tempQueue[j];
                    tempQueue[j] = temp;
                }
            }
        }

        // Enqueue the sorted jobs into a new queue
        for (int i = 0; i < size; ++i) {
            newQueue->enqueue(tempQueue[i]);
        }
        return newQueue;
    }

    // Method to display all jobs
    void display() const {
        for (int i = 0; i < size; ++i) {
            NodePtrs[i]->display();
        }
    }

    // Method to return the number of jobs in the queue
    int count() const {
        return size;
    }

    // Method to list jobs in order of job IDs
    void listJobs() const {
        for (int i = 0; i < size; ++i) {
            cout << "Position " << i << ": ";
            NodePtrs[i]->display();
        }
    }
};

// Main function
int main() {
    int n;  // Number of commands
    cin >> n;  // Read the number of commands
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();  // Fix instantiation
    char command;

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
                myNovelQueue->enqueue(newJob);  // Works with CPUJob* directly
                break;
            }
            case 'R': {  // Remove (Dequeue)
                CPUJob* removedJob = myNovelQueue->dequeue();  // Returns CPUJob*
                if (removedJob) {
                    cout << "Dequeued Job: ";
                    removedJob->display();
                    delete removedJob;
                }
                break;
            }
            case 'M': {  // Modify
                cin >> job_id >> new_priority >> new_job_type >> new_cpu_time >> new_memory;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time, new_memory);
                break;
            }
            case 'C': {  // Change Job Values
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                break;
            }
            case 'P': {  // Promote
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);
                break;
            }
            case 'O': {  // Reorder
                cin >> attribute_index;
                NovelQueue<CPUJob>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                cout << "Reordered Queue:" << endl;
                reorderedQueue->display();
                delete reorderedQueue;
                break;
            }
            case 'D': {  // Display
                myNovelQueue->display();
                break;
            }
            case 'N': {  // Count
                cout << "Number of jobs in the queue: " << myNovelQueue->count() << endl;
                break;
            }
            case 'L': {  // List Jobs
                myNovelQueue->listJobs();
                break;
            }
            default:
                cout << "Invalid command!" << endl;
        }
    }

    delete myNovelQueue;
    return 0;
}
