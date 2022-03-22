#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>

using namespace std;

// class to keep person's data
class Person
{
  public:
    string first_name, last_name;
    double Sy, Sx, Hx, Hy;
    // constructor accepts 6 arguments to instantiate person's object
    Person(string fn, string ln, double m1, double m2, double m3, double m4)
    {
        first_name = fn;
        last_name = ln;
        Sy = m1; //saddle height
        Sx = m2; //saddle setback
        Hx = m3; //grip reach (tip of saddle to grip x vector)
        Hy = m4; //grip drop (center of saddle to grip y vector)
    }
    Person() {} // default constructor

    // overloaded less operator - needed for binary tree construction.
    // in bst add() function there is a comparison between current node and given data
    bool operator<(const Person &p)
    {
        // firstly check last names, if name is less then compared, return true
        if (last_name < p.last_name)
        {
            return true;
        }
        else if (last_name > p.last_name) // otherwise, if  name is bigger return false
        {
            return false;
        }
        else // but if names are equal, compare measurment 1
        {
            return Sy < p.Sy; // if names are equal - compare Sy
        }
    }

    // checks if 2 person objects are equal. If yes - returns true, otherwise returns false
    // checks equality of both first name and last name
    bool operator==(const Person &p)
    {
        if (last_name == p.last_name) // 2 people are equal if and only if their names are equal, both first and last
        {
            if (first_name == p.first_name)
            {
                return true;
            }
        }
        return false;
    }
    // overloaded ostream operator is a friend function of Person class. It's convinient to use
    // this to output the data of the Person.
    // it's implementation is outside of the class
    friend ostream &operator<<(ostream &os, const Person &p);
};
// overloaded ostream operator used by Person class
ostream &operator<<(ostream &os, const Person &p)
{
    // writes all person's data and returns object of ostream data was written to
    os << p.first_name << " " << p.last_name << " " << p.Sy << " " << p.Sx << " " << p.Hx << " " << p.Hy;
    return os;
}

// Templated Binary tree class
template <class T>
class BinarySearchTree
{
    // inner struct - node of the tree
    struct Node
    {
        T data;
        Node *left, *right;
        // constructor of the node
        Node(T t, Node *l, Node *r)
        {
            data = t;
            left = l;
            right = r;
        }
    };
    // head of the tree
    Node *head;
    // recursive private function to add an element to the tree, can not be accessed outside of class
    void add(Node *&n, T d)
    {
        if (n == 0) // firstly check if Node n is 0 (it has no data yet) - just push given data to it and that's it
        {
            n = new Node(d, 0, 0);
            return;
        }
        if (d < n->data)
        { // if current element is less then node value - put it to the left
            add(n->left, d);
            return;
        }
        else
        { // otherwise put it to the right
            add(n->right, d);
            return;
        }
    }
    // recursively deletes all nodes of the tree
    void clear(Node *n)
    {
        if (n != 0) // checks if node can be deleted
        {
            clear(n->left); // firstly deletes all left children of this node
            clear(n->right); // next it deletes all right children of this node
            delete n; // and finally deletes node itself
        }
    }
    // returns object (not copy!) of the element stored in a tree, so you can modify it
    T *search(T data, Node *n)
    {
        // this works same as addition, but also checks if element is found and in that case returns it
        if (n == 0)
        {
            return 0;
        }
        if (n->data == data)
        {
            return &n->data; // found, return object
        }
        else if (n->data < data) // go to the left subtree
        {
            return search(data, n->right);
        }
        else // go to the right subtree
        {
            return search(data, n->left);
        }
    }
    // creates vector that holds all data from binary tree
    // data is stored in form of inorder traversal of the tree, so it is sorted in ascending order
    // this method is required to compare 10 closest peoples saddle heights
    void inorder_vector(Node *n, vector<T> &v)
    {
        // this function is pretty much the same as inorder traversal
        // but instead of printing the element, it adds it to the vector passed as a parameter
        if (n != 0)
        {
            inorder_vector(n->left, v);
            v.push_back(n->data);
            inorder_vector(n->right, v);
        }
    }

  public:

    // all following methods are wrappers for the private ones
    // this is because private ones require pointer to head of the tree


    // public function to add element. This one calls private one, which has more arguments and is recursive
    void add(T t)
    {
        add(head, t); // private method call, pass head as a starting node
    }
    // returns linearized tree, calls private method inorder_vector()
    vector<T> tree_to_vector()
    {
        vector<T> v; // create vector to put tree data in
        inorder_vector(head, v); // fill it in
        return v; // return it
    }
    // returns found (or not found) element
    T *search(T &data)
    {
        return search(data, head);
    }
    // constructor
    BinarySearchTree()
    {
        head = 0;
    }
    // destructor recursively deletes nodes by calling clear() function
    ~BinarySearchTree()
    {
        clear(head);
    }
};

// splits the string and returns array of tokens
std::vector<std::string> split(const std::string &s, const std::string &delim)
{
    std::vector<std::string> result; // vector to hold the result
    if (delim.empty()) // check if delimeter is empty - result is equal to the given string
    {
        result.push_back(s);
        return result;
    }
    std::string::const_iterator substart = s.begin(), subend; // iterator to iterate over a string
    while (true)
    {
        subend = search(substart, s.end(), delim.begin(), delim.end()); // searching first occurence of delimeter
        std::string temp(substart, subend); // create string of characters from start to delimiter position
        if (!temp.empty()) // check if this string not empty - push it to the result
        {
            result.push_back(temp);
        }
        if (subend == s.end()) // if end of the found substring is equal to the end of the given string -
        {                      // means that there is nothing to split anymore, return result
            break;
        }
        substart = subend + delim.size(); // move iterator to the position after first occurence of the delimeter
    }
    return result;
}
// comparator used to sort array of pairs in estimate function
bool cmp(pair<double, Person> &p1, pair<double, Person> &p2)
{
    return p1.first < p2.first;
}
// the idea is to create a vector of pairs of double and person
// double holds absolute difference between given inseam value and person's Sy
// the closest values to the inseam will be the values with least difference
// .first refers to persons difference in sy, .second refers to their sy
void estimate(double inseam, BinarySearchTree<Person> &tree)
{
    inseam *= 0.883; // LeMond Formula for finding Sy from inseam
    vector<pair<double, Person>> diff; // create new binary tree to hold differences in Sy of each person and inseam value
    vector<Person> v = tree.tree_to_vector(); // "linearize" tree
    for (size_t i = 0; i < v.size(); i++)
    {
        diff.push_back(make_pair(abs(inseam - v[i].Sy), v[i])); // add difference value to the vector, as well as corresponding Person object
    }
    sort(diff.begin(), diff.end(), cmp); // sort values using custom cmp method, which compares first element of a pair
    double avg2 = 0, avg3 = 0, avg4 = 0;
    // calculate averages
    for (int i = 0; i < 10; i++)
    {
        avg2 += diff[i].second.Sx; //.second is persons a persons sx
        avg3 += diff[i].second.Hx; //hx
        avg4 += diff[i].second.Hy; //hy
    }
    avg2 /= 10.0;
    avg3 /= 10.0;
    avg4 /= 10.0;
    // output results
    cout << "Based on the measurements of the 10 most similar people, your estimated bike fit is: " <<"Saddle Height: "<< inseam << ", " <<"Saddle Setback: "<< avg2 << ", " <<"Grip Reach: "<< avg3 << ", " <<"Grip Drop: "<< avg4 << endl;
}

int main()
{
    BinarySearchTree<Person> tree; //create tree
    ifstream fin("BodyData.csv"); // read file
    string line;
    while (getline(fin, line)) // while there is a non-empty line
    {
        vector<string> tokens; // to hold read data and create Person object
        tokens = split(line, ",");
        // construct person object on split string
        Person p(tokens[1], tokens[0], stod(tokens[2]), stod(tokens[3]), stod(tokens[4]), stod(tokens[5]));
        tree.add(p); // add person to the tree
    }
    fin.close(); // close file
    while (true) // shows menu
    {
        cout << "1) add rider" << endl;
        cout << "2) search rider" << endl;
        cout << "3) edit rider measurments" << endl;
        cout << "4) get bike fit estimate" << endl;
        cout << "5) exit" << endl;
        int option;
        cin >> option;
        switch (option)
        {
        case 1:
        {
            string fn, ln;
            double Sy, Sx, Hx, Hy;
            cout << "Enter first name: ";
            cin >> fn;
            cout << "Enter last name: ";
            cin >> ln;
            cout << "Enter measurments (Saddle Height, Saddle Setback, Grip Reach, Grip Drop, in mm, separated by spaces ): ";
            cin >> Sy >> Sx >> Hx >> Hy;
            // construct new person object based on given data
            Person p(fn, ln, Sy, Sx, Hx, Hy);
            // push it to the bst
            tree.add(p);
            break;
        }
        case 2:
        {
            string fn, ln;
            cout << "Enter first name: ";
            cin >> fn;
            cout << "Enter last name: ";
            cin >> ln;
            // construct new person object based on given data
            Person p(fn, ln, 0, 0, 0, 0); // fill mm's with 0, as they dont account when searching
            // search it in the bst and cout it
            Person *res = tree.search(p);
            if (res)
                cout << *res << endl; // use of overloaded ostream operator
            else {
                cout << "Not found" << endl;
            }
            break;
        }
        case 3:
        {
            string fn, ln;
            double Sy, Sx, Hx, Hy;
            cout << "Enter first name: ";
            cin >> fn;
            cout << "Enter last name: ";
            cin >> ln;
            cout << "Saddle Height ? ";
            cin >> Sy;
            cout << "Saddle Setback ? ";
            cin >> Sx;
            cout << "Grip Reach ? ";
            cin >> Hx;
            cout << "Grip Drop ? ";
            cin >> Hy;
            // construct new person object based on given data
            Person p(fn, ln, Sy, Sx, Hx, Hy);
            // update it's mm's in the tree
            Person *found = tree.search(p);
            if (found) {
                // update this node
                found->Sy = Sy;
                found->Sx = Sx;
                found->Hx = Hx;
                found->Hy = Hy;
                cout << p.first_name << " " << p.last_name << "'s measurments have been edited" << endl;
            } else {
                cout << p.first_name << " " << p.last_name << " was not found" << endl;
            }


            break;
        }
        case 4:
        {
            double inseam;
            cout << "Enter inseam value: " << endl;
            cin >> inseam;
            estimate(inseam, tree); // call function to produce result
            break;
        }
        case 5:
        {
            return 0;
        }
        default:
        {
            cout << "Invalid option, try again";
        }
        }
    }

    return 0;
}
