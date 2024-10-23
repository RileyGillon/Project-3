#include <iostream>
#include <vector>
#include <algorithm>  // for std::sort
using namespace std;

// CPUJob class definition
class CPUJob {
public:
    int job_id;
    int priority;
    int job_type;
    int cpu_time_consumed;
    int memory_consumed;

    CPUJob(int id, int prio, int type, int cpu_time, int memory)
        : job_id(id), priority(prio), job_type(type), cpu_time_consumed(cpu_time), memory_consumed(memory) {}

    void display() {
        cout << "Job ID: " << job_id
             << ", Priority: " << priority
             << ", Job Type: " << job_type
             << ", CPU Time Consumed: " << cpu_time_consumed
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

// Queue node class definition
template <class DT>
class Queue {
public:
    DT JobPointer;
    Queue* next;

    // The constructor should take DT (which will be CPUJob* in our case)
    Queue(DT job) : JobPointer(job), next(nullptr) {}
};

// NovelQueue class definition
template <class DT>
class NovelQueue {
public:
    Queue<DT>* front;
    int size;

    NovelQueue() : front(nullptr), size(0) {}

    // Enqueue method to add a job to the queue
    void enqueue(DT job) {
        Queue<DT>* newNode = new Queue<DT>(job);

        if (!front) {
            front = newNode;
        } else {
            Queue<DT>* temp = front;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    // Dequeue method to remove and return the first job from the queue
    DT dequeue() {
        if (!front) return nullptr;  // If the queue is empty, return nullptr

        Queue<DT>* temp = front;
        front = front->next;

        DT job = temp->JobPointer;  // This is now correctly of type 'DT' (which is 'CPUJob*')
        delete temp;
        size--;

        return job;
    }

    // Modify a job by job ID
    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                temp->JobPointer->priority = new_priority;
                temp->JobPointer->job_type = new_job_type;
                temp->JobPointer->cpu_time_consumed = new_cpu_time_consumed;
                temp->JobPointer->memory_consumed = new_memory_consumed;
                return;
            }
            temp = temp->next;
        }
    }

    // Change a specific field of a job
    void change(int job_id, int field_index, int new_value) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                switch (field_index) {
                    case 1:
                        temp->JobPointer->priority = new_value;
                        break;
                    case 2:
                        temp->JobPointer->job_type = new_value;
                        break;
                    case 3:
                        temp->JobPointer->cpu_time_consumed = new_value;
                        break;
                    case 4:
                        temp->JobPointer->memory_consumed = new_value;
                        break;
                    default:
                        cout << "Invalid field index!" << endl;
                        return;
                }
                return;
            }
            temp = temp->next;
        }
    }

    // Promote a job within the queue
    void promote(int job_id, int positions) {
        if (positions <= 0 || front == nullptr || front->next == nullptr) {
            return; // No need to promote if positions is non-positive or queue is empty/single element
        }

        Queue<DT>* prev = nullptr;
        Queue<DT>* curr = front;

        // Find the job in the queue
        while (curr != nullptr && curr->JobPointer->job_id != job_id) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            return; // Job not found
        }

        if (prev == nullptr) {
            return; // Job is already at the front
        }

        // Remove the job from its current position
        prev->next = curr->next;

        // Move the job to its new position
        Queue<DT>* temp = front;
        Queue<DT>* tempPrev = nullptr;
        for (int i = 0; i < positions && temp != nullptr; ++i) {
            tempPrev = temp;
            temp = temp->next;
        }

        if (tempPrev == nullptr) {
            curr->next = front;
            front = curr;
        } else {
            curr->next = tempPrev->next;
            tempPrev->next = curr;
        }
    }

    // Display all jobs in the queue
    void display() {
        Queue<DT>* temp = front;
        while (temp) {
            temp->JobPointer->display();  // Assuming CPUJob has a 'display' function
            temp = temp->next;
        }
    }

    // Count the number of jobs in the queue
    int count() {
        return size;
    }

    // List all jobs in the queue
    void listJobs() {
        display();  // This can be similar to display, but perhaps with extra formatting
    }

    // Reorder jobs in the queue based on a specified attribute
    NovelQueue<DT>* reorder(int attribute_index) {
        // Create a new queue to store reordered jobs
        NovelQueue<DT>* reorderedQueue = new NovelQueue<DT>();

        // Copy all jobs to a vector for sorting
        vector<DT> jobs;
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            jobs.push_back(temp->JobPointer);
            temp = temp->next;
        }

        // Sort the jobs based on the specified attribute using std::sort
        std::sort(jobs.begin(), jobs.end(), [attribute_index](DT a, DT b) {
            switch (attribute_index) {
                case 1: return a->priority < b->priority;
                case 2: return a->job_type < b->job_type;
                case 3: return a->cpu_time_consumed < b->cpu_time_consumed;
                case 4: return a->memory_consumed < b->memory_consumed;
                default: return false;
            }
        });

        // Enqueue the sorted jobs into the new queue
        for (DT job : jobs) {
            reorderedQueue->enqueue(job);
        }

        return reorderedQueue;
    }
};

// Main function
int main() {
    int n;
    cin >> n;

    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    char command;

    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
    int new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed;
    int field_index, new_value;
    int positions;
    int attribute_index;

    for (int i = 0; i < n; ++i) {
        cin >> command;

        switch (command) {
            case 'A': {  // Add a new job
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue->enqueue(newJob);
                cout << "Enqueued Job: " << endl;
                cout << "Job ID: " << newJob->job_id 
                << ", Priority: " << newJob->priority 
                << ", Job Type: " << newJob->job_type 
                << ", CPU Time Consumed: " << newJob->cpu_time_consumed 
                << ", Memory Consumed: " << newJob->memory_consumed << endl;
                cout << "Jobs after enqueue: " << endl;
                myNovelQueue->display();
                break;
            }
            case 'R': {  // Remove the first job from the queue
                CPUJob* removedJob = myNovelQueue->dequeue();
                if (removedJob) {
                    cout << "Dequeued Job: " << endl;
                    removedJob->display();
                    delete removedJob;
                }
                cout << "Jobs after dequeue: " << endl;
                myNovelQueue->display();
                break;
            }
            case 'M': {  // Modify an existing job
                cin >> job_id >> new_priority >> new_job_type >> new_cpu_time_consumed >> new_memory_consumed;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                break;
            }
            case 'C': {  // Change a specific field of a job
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                break;
            }
            case 'P': {  // Promote a job within the queue
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);
                cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s): " << endl;
                cout << "Jobs after promotion: " << endl;
                myNovelQueue->display();
                break;
            }
            case 'O': {  // Reorder the queue based on a specific attribute
                cin >> attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                cout << "Reordered Queue:" << endl;
                reorderedQueue->display();
                delete reorderedQueue; // Avoid memory leak
                break;
            }
            case 'D': {  // Display all jobs in the queue
                myNovelQueue->display();
                break;
            }
            case 'N': {  // Display the number of jobs in the queue
                cout << "Number of elements in the queue: " << myNovelQueue->count() << endl;
                break;
            }
        }
    }

    // Clean up the queue
    delete myNovelQueue;

    return 0;
}
