#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
    Course() {}
};

struct Node {
    Course course;
    Node* left;
    Node* right;
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        } else if (course.courseNumber < node->course.courseNumber) {
            addNode(node->left, course);
        } else {
            addNode(node->right, course);
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.title << endl;
            inOrder(node->right);
        }
    }

    Node* search(Node* node, string courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber) {
            return node;
        } else if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        } else {
            return search(node->right, courseNumber);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroyTree(root); }

    void Insert(Course course) {
        addNode(root, course);
    }

    void PrintAll() {
        inOrder(root);
    }

    void PrintCourse(string courseNumber) {
        Node* node = search(root, courseNumber);
        if (node == nullptr) {
            cout << "Course not found." << endl;
        } else {
            cout << node->course.courseNumber << ", " << node->course.title << endl;
            if (!node->course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < node->course.prerequisites.size(); ++i) {
                    cout << node->course.prerequisites[i];
                    if (i < node->course.prerequisites.size() - 1) cout << ", ";
                }
                cout << endl;
            }
        }
    }
};

Course ParseCourse(string line) {
    stringstream ss(line);
    string token;
    Course course;

    getline(ss, token, ',');
    course.courseNumber = token;

    getline(ss, token, ',');
    course.title = token;

    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            course.prerequisites.push_back(token);
        }
    }

    return course;
}

void LoadData(string fileName, BinarySearchTree& bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Cannot open file." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            Course course = ParseCourse(line);
            bst.Insert(course);
        }
    }
    file.close();
    cout << "Data loaded successfully." << endl;
}

int main() {
    BinarySearchTree bst;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter file name: ";
            getline(cin, fileName);
            LoadData(fileName, bst);
            break;
        case 2:
            cout << "\nHere is a sample schedule:" << endl;
            bst.PrintAll();
            break;
        case 3:
            cout << "What course do you want to know about? ";
            getline(cin, courseNumber);
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
            bst.PrintCourse(courseNumber);
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
