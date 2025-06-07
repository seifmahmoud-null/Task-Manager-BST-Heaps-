
#ifndef HEAP_H
#define HEAP_H
#include "BST.h"

class MinHeap {
private:
    Task* HeapArray;
    int size;
    int capacity;

    //doubles array size in case capacity reached its limit
    void resizeHeap() {
        capacity *= 2;
        Task* NewArray = new Task[capacity];

        for (int i = 0; i < size; i++) {
            NewArray[i] = HeapArray[i];
        }
        delete[] HeapArray;
        HeapArray = NewArray;
    }

    //swap two task objects
    void swap(Task& a, Task& b) {
        Task Temp = a;
        a = b;
        b = Temp;
    }

    public:

    //constructors and getters
    MinHeap(int capacity = 10) {
        this->capacity = capacity;
        this->size = 0;
        this->HeapArray = new Task[capacity];
    }
    ~MinHeap() {delete[] HeapArray;}
    int getSize() const { return size; }
    Task* getArray() const { return HeapArray; }

    //heapify down, compares with children and swaps accordingly and recurses
    void MinHeapify(Task* A, int index, int length) {
        int left = (index * 2) + 1;
        int right = (index * 2) + 2;
        int smallest;
        if(left < length && A[index].getDuration() > A[left].getDuration()) {
            smallest = left;
        }
        else {
            smallest = index;
        }
        if(right < length && A[smallest].getDuration() > A[right].getDuration()) {
            smallest = right;
        }
        if(smallest != index) {
            swap(A[smallest], A[index]);
            MinHeapify(A, smallest, length);
        }
    }
    //heapify down, compares with parent and swaps accordingly and recurses
    void MinHeapifyUp(Task* A, int index) {
        while(index > 0 && A[(index-1)/2].getDuration() > A[index].getDuration()) {
            int parent = (index - 1) / 2;
            swap(A[index], A[parent]);
            index = parent;
        }
    }

    //insert task to be used in mark task as complete
    void insert(Task& task) {
        if(size == capacity) {
            resizeHeap();
        }
        HeapArray[size] = task;
        MinHeapifyUp(HeapArray, size);
        size++;
    }

    //removes the minimum duration(root), to be used in displaycompletedtasks
    Task removeMin() {
        Task Min = HeapArray[0];
        HeapArray[0] = HeapArray[size-1];
        size--;
        MinHeapify(HeapArray, size, capacity);
        return Min;
    }

    void displayCompletedTasks() {
        if (size == 0) {
            cout << "No completed tasks." << endl;
            return;
        }

        MinHeap tempHeap(size);
        //create a temp heap and copy the HeapArray so that we can display without delete the array
        for (int i = 0; i < size; i++) {
            tempHeap.getArray()[i] = HeapArray[i];
        }
        tempHeap.size = size; // set the size correctly

        //predefined categories
        const int numCategories = 5;
        string categories[numCategories] = {
            "Educational", "Health", "Self development", "Food", "Other"
        };
        int categoryCount[numCategories] = {0};

        //display tasks and count per category
        cout << "\ncompleted tasks:\n";
        for (int i = 0; i < size; i++) {
            Task tempTask = tempHeap.removeMin();
            cout << "[" << tempTask.getDescription() << ", "
                 << tempTask.getDuration() << ", "
                 << tempTask.getCategory() << "]\n";

            //match task's category to predefined list
            string cat = tempTask.getCategory();
            for (int j = 0; j < numCategories; j++) {
                if (categories[j] == cat) {
                    categoryCount[j]++;
                    break;
                }
            }
        }
        //print report
        cout << "\nCategory Report\n";
        for (int i = 0; i < numCategories; i++) {
            cout << categories[i] << " = " << categoryCount[i] << endl;
        }
    }

    //to be able to use markTaskCompleted function, i had to include heap.h in BST.h which cause
    //several inexplainable problems with the compiler, thus this function's implementation
    //will be found in main.cpp
};
#endif
