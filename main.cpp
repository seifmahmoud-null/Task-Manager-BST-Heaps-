
#include <iostream>
using namespace std;
#include "BST.h"
#include "heap.h"
#include <fstream>
#include <limits>
#include <fstream>
#include <iostream>
using namespace std;

//create tasks file that was given as an example in the assignment
fstream createTasksFile() {
    fstream outFile("tasks.txt", ios::out);
    if (!outFile) {
        cout << "Error creating file!" << endl;
        return nullptr;
    }
    outFile << "10\n";
    outFile << "Study data structure\n";
    outFile << "60\n";
    outFile << "Educational\n";
    outFile << "Go the gym\n";
    outFile << "30\n";
    outFile << "Health\n";
    outFile << "Watch a podcast\n";
    outFile << "120\n";
    outFile << "Self development\n";
    outFile << "Prepare a Meal\n";
    outFile << "30\n";
    outFile << "Food\n";
    outFile << "Study software engineering\n";
    outFile << "60\n";
    outFile << "Educational\n";
    outFile << "Reading a book\n";
    outFile << "20\n";
    outFile << "Self development\n";
    outFile << "Tidy room\n";
    outFile << "15\n";
    outFile << "Other\n";
    outFile << "Shopping\n";
    outFile << "20\n";
    outFile << "Other\n";
    outFile << "Go to Library\n";
    outFile << "30\n";
    outFile << "Other\n";
    outFile << "Make DS Assignment\n";
    outFile << "100\n";
    outFile << "Educational\n";

    outFile.close();
    cout << "tasks.txt file created successfully.\n";
    return outFile;
}

// Helper to convert string to lowercase
string toLower(const string& s) {
    string result = s;
    for (char& c : result) c = tolower(c);
    return result;
}

// mark task as completed
void markTaskCompleted(BST& bst, MinHeap& heap, int duration, const string& descPart) {
    int count = 0;
    //first search for the existence of the task based on duration
    Task* matches = bst.searchTask(duration, count);
    if (!matches || count == 0) {
        cout << "No tasks found with that duration.\n";
        return;
    }

    //turn string to lowercase
    string target = toLower(descPart);
    bool found = false;

    for (int i = 0; i < count; i++) {
        //turn task description to lower to be able to compare
        string taskDesc = toLower(matches[i].getDescription());

        if (taskDesc.find(target) != string::npos) {
            // Found match
            cout << "[" << matches[i].getDescription() << ", "
                 << matches[i].getDuration() << ", "
                 << matches[i].getCategory() << "]\n";
            cout << "This task is removed\n";

            // Remove and insert into heap
            bst.removeTask(duration);
            heap.insert(matches[i]);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No task matched the given description.\n";
    }

    delete[] matches;
}

//main menu
int main() {
    BST bst;
    MinHeap completedHeap;
    fstream file = createTasksFile();
    bst.readFromFile(file);
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Insert a task (Using BST class)\n";
        cout << "2. Display all tasks (BST)\n";
        cout << "3. Search for a task by duration (BST)\n";
        cout << "4. Remove a task by duration (BST)\n";
        cout << "5. Display tasks with duration more than (BST)\n";
        cout << "6. Display tasks with duration less than (BST)\n";
        cout << "7. Mark a task as completed by task duration and description (using heap Class)\n";
        cout << "8. Display all completed tasks and the number of tasks completed per category (using heap Class)\n";
        cout << "0. Exit\n";

        int choice;
        cout << "Enter number of option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard leftover newline

        if (choice == 0) break;

        string description, category;
        int duration;

        switch (choice) {

            case 1:
                cout << "Enter task description: ";
                getline(cin, description);
                cout << "Enter duration: ";
                cin >> duration;
                cin.ignore();
                cout << "Enter category: ";
                getline(cin, category);
                bst.insertTask(description, duration, category);
                cout << "The task is added.\n";
                break;

            case 2:
                bst.displayAll();
                break;

            case 3:
                cout << "Enter duration to search: ";
                cin >> duration;
                {
                    int count;
                    Task* results = bst.searchTask(duration, count);
                    if (count == 0) cout << "No tasks found.\n";
                    else {
                        for (int i = 0; i < count; i++) {
                            cout << "[" << results[i].getDescription() << ", "
                                 << results[i].getDuration() << ", "
                                 << results[i].getCategory() << "]\n";
                        }
                    }
                    delete[] results;
                }
                break;

            case 4:
                cout << "Enter duration to remove: ";
                cin >> duration;
                bst.removeTask(duration);
                cout << "Task(s) removed if any existed.\n";
                break;

            case 5:
                cout << "Enter the duration more than: ";
                cin >> duration;
                bst.displayMoreThan(duration);
                break;

            case 6:
                cout << "Enter the duration less than: ";
                cin >> duration;
                bst.displayLessThan(duration);
                break;

            case 7:
                cout << "Task duration: ";
                cin >> duration;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard leftover newline
                cout << "Task description: ";
                getline(cin, description);
                markTaskCompleted(bst, completedHeap, duration, description);
                break;

            case 8:
                completedHeap.displayCompletedTasks();
                break;

            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}