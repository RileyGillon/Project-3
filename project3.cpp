#include <iostream>
#include <string>
using namespace std;

// CPUJob class to store job information
class CPUJob {
private:
    int job_id;
    int priority;
    int job_type;
    int cpu_time_consumed;
    int memory_consumed;

public:
    // Constructor
    CPUJob(int id, int p, int type, int cpu_time, int mem) : 
        job_id(id), priority(p), job_type(type), 
        cpu_time_consumed(cpu_time), memory_consumed(mem) {}

    // Getters
    int getJobId() const { return job_id; }
    int getPriority() const { return priority; }
    int getJobType() const { return job_type; }
    int getCPUTime() const { return cpu_time_consumed; }
    int getMemory() const { return memory_consumed; }

    // Setters
    void setPriority(int p) { priority = p; }
    void setJobType(int t) { job_type = t; }
    void setCPUTime(int t) { cpu_time_consumed = t; }
    void setMemory(int m) { memory_consumed = m; }

    // Display method
    void display() const {
        cout << "Job ID: " << job_id 
             << ", Priority: " << priority 
             << ", Job Type: " << job_type 
             << ", CPU Time Consumed: " << cpu_time_consumed 
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

// Queue node template class
template <class DT>
class Queue {
public:
    DT JobPointer;
    Queue<DT>* next;

    Queue(DT job) : JobPointer(job), next(nullptr) {}
};

// NovelQueue template class
template <class DT>
class NovelQueue {
private:
    Queue<DT>* front;
    Queue<DT>** NodePtrs;
    int size;
    int capacity;

    void resizeNodePtrs() {
        int newCapacity = capacity * 2;
        Queue<DT>** newPtrs = new Queue<DT>*[newCapacity];
        
        for (int i = 0; i < size; i++) {
            newPtrs[i] = NodePtrs[i];
        }
        
        delete[] NodePtrs;
        NodePtrs = newPtrs;
        capacity = newCapacity;
    }

public:
    NovelQueue() : front(nullptr), size(0), capacity(10) {
        NodePtrs = new Queue<DT>*[capacity];
    }

    ~NovelQueue() {
        while (front != nullptr) {
            Queue<DT>* temp = front;
            front = front->next;
            delete temp;
        }
        delete[] NodePtrs;
    }

    // Method to get job at specific position
    DT getJobAtPosition(int pos) const {
        if (pos >= 0 && pos < size) {
            return NodePtrs[pos]->JobPointer;
        }
        return nullptr;
    }

    int binarySearch(int job_id) const {
        int left = 0;
        int right = size - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int midJobId = NodePtrs[mid]->JobPointer->getJobId();
            
            if (midJobId == job_id)
                return mid;
            if (midJobId < job_id)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -1;
    }

    void enqueue(DT job) {
        Queue<DT>* newNode = new Queue<DT>(job);
        
        if (size == capacity) {
            resizeNodePtrs();
        }

        // Insert into linked list
        if (front == nullptr) {
            front = newNode;
        } else {
            Queue<DT>* current = front;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }

        // Insert into sorted array
        int insertPos = 0;
        while (insertPos < size && 
               NodePtrs[insertPos]->JobPointer->getJobId() < job->getJobId()) {
            insertPos++;
        }

        for (int i = size; i > insertPos; i--) {
            NodePtrs[i] = NodePtrs[i - 1];
        }
        NodePtrs[insertPos] = newNode;
        size++;
    }

    DT dequeue() {
        if (front == nullptr) {
            cout << "Queue is empty" << endl;
            return nullptr;
        }

        Queue<DT>* temp = front;
        DT job = temp->JobPointer;
        front = front->next;

        // Remove from NodePtrs
        int pos = binarySearch(job->getJobId());
        if (pos != -1) {
            for (int i = pos; i < size - 1; i++) {
                NodePtrs[i] = NodePtrs[i + 1];
            }
            size--;
        }

        delete temp;
        return job;
    }

    void modify(int job_id, int new_priority, int new_job_type, 
                int new_cpu_time, int new_memory) {
        int pos = binarySearch(job_id);
        if (pos != -1) {
            NodePtrs[pos]->JobPointer->setPriority(new_priority);
            NodePtrs[pos]->JobPointer->setJobType(new_job_type);
            NodePtrs[pos]->JobPointer->setCPUTime(new_cpu_time);
            NodePtrs[pos]->JobPointer->setMemory(new_memory);
        }
    }

    void change(int job_id, int field_index, int new_value) {
        int pos = binarySearch(job_id);
        if (pos != -1) {
            switch (field_index) {
                case 1:
                    NodePtrs[pos]->JobPointer->setPriority(new_value);
                    break;
                case 2:
                    NodePtrs[pos]->JobPointer->setJobType(new_value);
                    break;
                case 3:
                    NodePtrs[pos]->JobPointer->setCPUTime(new_value);
                    break;
                case 4:
                    NodePtrs[pos]->JobPointer->setMemory(new_value);
                    break;
            }
        }
    }

    void promote(int job_id, int positions) {
        Queue<DT>* current = front;
        Queue<DT>* prev = nullptr;
        int currentPos = 0;

        // Find the job and count its position
        while (current != nullptr && current->JobPointer->getJobId() != job_id) {
            prev = current;
            current = current->next;
            currentPos++;
        }

        if (current == nullptr || currentPos < positions) return;

        // Remove from current position
        if (prev) {
            prev->next = current->next;
        } else {
            return; // Already at front
        }

        // Insert at new position
        if (positions == currentPos) {
            current->next = front;
            front = current;
        } else {
            Queue<DT>* targetPrev = front;
            for (int i = 0; i < currentPos - positions - 1; i++) {
                targetPrev = targetPrev->next;
            }
            current->next = targetPrev->next;
            targetPrev->next = current;
        }
    }

    NovelQueue<DT>* reorder(int attribute_index) {
        NovelQueue<DT>* newQueue = new NovelQueue<DT>();
        
        // Copy all jobs to an array for sorting
        DT* jobs = new DT[size];
        Queue<DT>* current = front;
        int index = 0;
        
        while (current != nullptr) {
            jobs[index++] = current->JobPointer;
            current = current->next;
        }

        // Sort based on attribute
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                bool shouldSwap = false;
                
                switch (attribute_index) {
                    case 1: // Priority
                        shouldSwap = jobs[j]->getPriority() > jobs[j + 1]->getPriority();
                        break;
                    case 2: // Job Type
                        shouldSwap = jobs[j]->getJobType() > jobs[j + 1]->getJobType();
                        break;
                    case 3: // CPU Time
                        shouldSwap = jobs[j]->getCPUTime() > jobs[j + 1]->getCPUTime();
                        break;
                    case 4: // Memory
                        shouldSwap = jobs[j]->getMemory() > jobs[j + 1]->getMemory();
                        break;
                }
                
                if (shouldSwap) {
                    DT temp = jobs[j];
                    jobs[j] = jobs[j + 1];
                    jobs[j + 1] = temp;
                }
            }
        }

        // Create new queue with sorted jobs
        for (int i = 0; i < size; i++) {
            newQueue->enqueue(jobs[i]);
        }

        delete[] jobs;
        return newQueue;
    }

    void display() const {
        Queue<DT>* current = front;
        while (current != nullptr) {
            current->JobPointer->display();
            current = current->next;
        }
    }

    int count() const {
        return size;
    }

    void listJobs() const {
        cout << "List of jobs sorted by job IDs:" << endl;
        for (int i = 0; i < size; i++) {
            NodePtrs[i]->JobPointer->display();
        }
    }
};

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
            case 'A': {
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue->enqueue(newJob);
                cout << "Enqueued Job: " << endl;
                newJob->display();
                cout << "Jobs after enqueue: " << endl;
                myNovelQueue->display();
                break;
            }
            case 'R': {
                CPUJob* removedJob = myNovelQueue->dequeue();
                if (removedJob) {
                    cout << "Dequeued Job:" << endl;
                    removedJob->display();
                    cout << "Jobs after dequeue:" << endl;
                    myNovelQueue->display();
                    delete removedJob;
                }
                break;
            }
            case 'M': {
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                cout << "Modified Job ID " << job_id << ":" << endl;
                cout << "Jobs after modification:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'C': {
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                cout << "Changed Job ID "<< job_id << " field " << field_index << " to " << new_value << ":" << endl;
                cout << "Jobs after changing field:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'P': {
                cin >> job_id >> positions;
                int pos = myNovelQueue->binarySearch(job_id);
                if (pos != -1) {
                    CPUJob* jobToPromote = myNovelQueue->getJobAtPosition(pos);
                    if (jobToPromote) {
                        cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s): " << endl;
                        jobToPromote->display();
                    }
                }
                myNovelQueue->promote(job_id, positions);
                cout << "Jobs after promotion: " << endl;
                myNovelQueue->display();
                break;
            }
            case 'O': {
                cin >> attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                cout << "Reordered Queue by attribute " << attribute_index << ":" << endl;
                reorderedQueue->display();
                delete reorderedQueue;
                break;
            }
            case 'D': {
                cout << "Displaying all jobs in the queue:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'N': {
                cout << "Number of elements in the queue: " << myNovelQueue->count() << endl;
                break;
            }
            case 'L': {
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
