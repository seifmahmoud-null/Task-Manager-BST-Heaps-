
#ifndef BST_H
#define BST_H

#include <string>
#include <fstream>


using namespace std;

class Task {
private:
    string description;
    int duration;
    string category;
public:
    //getters and constructors
    int getDuration() const {return duration;}
    string getDescription() const {return description;}
    string getCategory() const {return category;}
    Task() : description(""), duration(0), category("") {}
    Task(string description, int duration, string category) :
        description(description), duration(duration), category(category) {}
};

class BSTNode {
private:
    Task key;
    BSTNode *left, *right;
public:
    //constructors
    BSTNode() : left(nullptr), right(nullptr) {}
    BSTNode(const Task& data, BSTNode *l=nullptr, BSTNode *r=nullptr) :
        key(data), left(l), right(r) {}

    //setters and getters
    Task& getKey() { return key; }
    void setKey(const Task& key) { this->key = key; }
    BSTNode*& getLeft()  { return left; }
    void setLeft(BSTNode* left) { this->left = left; }
    BSTNode*& getRight()  { return right; }
    void setRight(BSTNode* right) { this->right = right; }
};


class BST {
private:
    BSTNode *root;
    //recursion and delete node
    void clearHelper(BSTNode *node) {
        if (node != nullptr) {
            clearHelper(node->getLeft());
            clearHelper(node->getRight());
            delete node;
        }
    }

    //delete by copying function to be used in remove task
    void deleteByCopying(BSTNode*& node) {
        if (node == nullptr) return;

        if (node->getLeft() == nullptr) {
            BSTNode* temp = node;
            node = node->getRight();
            delete temp;
        }
        else if (node->getRight() == nullptr) {
            BSTNode* temp = node;
            node = node->getLeft();
            delete temp;
        }
        else {
            //find inorder predecessor (rightmost in left subtree)
            BSTNode* predParent = node;
            BSTNode* pred = node->getLeft();
            while (pred->getRight() != nullptr) {
                predParent = pred;
                pred = pred->getRight();
            }
            //copy predecessor's key to current node
            node->getKey() = pred->getKey();

            //delete predecessor node (which has at most left child)
            if (predParent == node) {
                // predecessor is direct left child
                predParent->setLeft(pred->getLeft());
            } else {
                predParent->setRight(pred->getLeft());
            }
            delete pred;
        }
    }


    //helper function for remove task
    void removeHelper(BSTNode*& node, int duration) {
        if (!node) return;

        if (duration < node->getKey().getDuration()) {
            removeHelper(node->getLeft(), duration);
        }
        else if (duration > node->getKey().getDuration()) {
            removeHelper(node->getRight(), duration);
        }
        else { // found node to delete
            deleteByCopying(node);
        }
    }

    //search for duration, and return a pointer of dynamic array of all matching tasks
    //also takes variable out_count (passed by reference, and modifies it so we know the size of the returned array
    //in case we need to use it later
    Task* searchTaskHelper(BSTNode* root_node, int duration, int& out_count) const {
        //first pass: count matches
        BSTNode* current = root_node;
        while (current) {
            if (duration == current->getKey().getDuration()) {
                out_count++;
                current = current->getLeft(); //equal durations are left children
            }
            else if (duration < current->getKey().getDuration()) {
                current = current->getLeft();
            }
            else {
                current = current->getRight();
            }
        }

        if (out_count == 0) return nullptr;

        //allocate array
        Task* results = new Task[out_count];
        int index = 0;
        current = root_node;

        //second pass: collect matches
        while (current && index < out_count) {
            if (duration == current->getKey().getDuration()) {
                results[index++] = current->getKey();
                current = current->getLeft();
            }
            else if (duration < current->getKey().getDuration()) {
                current = current->getLeft();
            }
            else {
                current = current->getRight();
            }
        }
        return results;
    }

    //display all (in order)
    void displayAllHelper(BSTNode *node) {
        if (node == nullptr) return;
        displayAllHelper(node->getLeft());
        cout << "[" << node->getKey().getDescription() << ", ";
        cout << node->getKey().getDuration() << ", ";
        cout << node->getKey().getCategory() << "]" << endl;
        displayAllHelper(node->getRight());
    }

    //display tasks with more duration than the given duration in the parameter
    void displayMoreThanHelper(BSTNode *node, int duration) {
        if (node == nullptr) return;
        displayMoreThanHelper(node->getLeft(), duration);
        if (node->getKey().getDuration() >= duration) {
            cout << "[" << node->getKey().getDescription() << ", ";
            cout << node->getKey().getDuration() << ", ";
            cout << node->getKey().getCategory() << "]" << endl;
        }
        displayMoreThanHelper(node->getRight(), duration);
    }

    //display tasks with less duration than the given duration in the parameter
    void displayLessThanHelper(BSTNode *node, int duration) {
        if (node == nullptr) return;
        displayLessThanHelper(node->getLeft(), duration);
        if (node->getKey().getDuration() <= duration) {
            cout << "[" << node->getKey().getDescription() << ", ";
            cout << node->getKey().getDuration() << ", ";
            cout << node->getKey().getCategory() << "]" << endl;
        }
        displayLessThanHelper(node->getRight(), duration);
    }

    //insert a task given the task parameters, while maintaining BST structure
    void insertTaskHelper(string description, int duration, string category, BSTNode*& node) {
        if (node == nullptr) {
            node = new BSTNode(Task(description, duration, category));
            return;
        }

        BSTNode* current = node;
        BSTNode* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (duration <= current->getKey().getDuration()) {
                current = current->getLeft();
            } else {
                current = current->getRight();
            }
        }

        if (duration <= parent->getKey().getDuration()) {
            parent->setLeft(new BSTNode(Task(description, duration, category)));
        } else {
            parent->setRight(new BSTNode(Task(description, duration, category)));
        }
    }


public:
    BST(BSTNode *node = 0) {root = node;}
    ~BST(){clear();}
    void clear() {clearHelper(root);}
    bool isEmpty(){return root == 0;}


    //insert task function which inserts a new task to the BST based on its duration value
    void insertTask(string description, int duration, string category) {
        insertTaskHelper(description, duration, category, root);
    }

    //remove task function
    void removeTask(int duration) {
        int n;
        // Keep deleting as long as a node with that duration exists
        while (searchTask(duration, n) != nullptr) {
            removeHelper(root, duration);
        }
    }

    //search for duration, and return a dynamic array of all matching tasks
    //also takes variable out_count (passed by reference, and modifies it so we know the size of the returned array
    Task* searchTask(int duration, int& out_count) const {
        out_count = 0;
        if (!root) return nullptr;
        return searchTaskHelper(root, duration, out_count);
    }
    void displayAll() {displayAllHelper(root);}
    void displayMoreThan(int duration) {displayMoreThanHelper(root, duration);}
    void displayLessThan(int duration) {displayLessThanHelper(root, duration);}

    //function to read from input file
    void readFromFile(fstream &inFile) {
        inFile.open("tasks.txt", ios::in);
        if (!inFile) {
            cout << "File not found: " <<  endl;
            return;
        }

        int taskCount;
        inFile >> taskCount;
        inFile.ignore(); // To ignore newline after task count

        for (int i = 0; i < taskCount; i++) {
            string description, category;
            int duration;

            getline(inFile, description);
            inFile >> duration;
            inFile.ignore();
            getline(inFile, category);

            insertTask(description, duration, category);
        }

        inFile.close();
        cout << taskCount << " tasks loaded from file.\n";
    }
};



#endif