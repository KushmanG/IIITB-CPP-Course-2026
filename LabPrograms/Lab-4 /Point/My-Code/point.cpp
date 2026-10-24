
// Library Imports and namespace
#include <iostream>
#include <string>
#include "point.h"
using namespace std;

// Initializing points array and total point count
int Point::total_point_count = 0;
Point* Point::all_point_objects[MAX_POINTS] = {nullptr};

// Getter functions
// Getter for getting the number of points in the point array
int Point::getPointCount() {
    return total_point_count;
}

// Getter for getting the array of all points
Point *Point::getPointObjects() {
    return *all_point_objects;
}

// Function to print out all the points
void Point::showAllPointObjects() {
    cout << "=========== POINTS REPO =============" << endl;
    for (int i = 0; i < total_point_count; i++) all_point_objects[i]->show();
    cout << "=========== POINTS REPO END =============" << endl;
}

// Constructor
Point::Point() {
    all_point_objects[total_point_count] = this;
    total_point_count++;
}

// Overload Constructor
/*
We make overload constructor for different types of object intializations,
the object chooses its constructor based off the format, 
Overload constructor and normal constructor must always differ in arguments
*/
Point::Point(string &alias, double x, double y) {
    // fill in: show initialize the data members properly, should update the static data members of Point class properly
    this->x = x;
    this->y = y;

    // For constructing the alias we perform a DEEP COPY VIA HEAP ALLOCATION
    /*
    This is necessary so that any change in the alias pointer doesn't change the initialized
    value,
    In general for any new string allocation on the heap we gotta do:   
        member_pointer = new string(S);
    
    And even more generally to make the deep copy of any object:
        Object* object_copy = new Object(*object);
        or 
        Object object_copy = new Object(object);
    */
    this->alias = new string(alias);

    // Now again an Overloaded constructor DOES NOT INHERIT THINGS FROM OTHER CONSTRUCTORS
    // and this operation is important, appending the point to the points array, thus we need to do this:
    all_point_objects[total_point_count] = this;
    total_point_count++;
}

Point::~Point() {
    // delete in case there is heap memory in the data members before updating statics
    /*
    This program is clearly wrong because there is heap memory allocation
    all_point_objects[total_point_count] = nullptr;
    total_point_count--;
    */

    // Firstly lets delete the alias deep copy we allocated on the heap
    delete this->alias;
    
    // Now we really don't know the position of the "this" pointer in the array
    // Assuming that it is at the total_point_count-th index could be false

    // First lets search for it in the array
    int this_index = -1;
    for (int i = 0; i < total_point_count; i++)
    {
        if (all_point_objects[i] == this) 
        {
            this_index = i;
            break;
        }
    }

    /*
    NOTE:
        this_index = -1 is just a random initialization I did for no reason, "thinking"
        there could be a fail case but also knowing obviously there couldn't so js don't
        mind it, simple int this_index; initialization is fine but I have PTSD cz many
        times do I get such stupid initialization errors so I js did it anyway, 
        I don't think I need a check case for this_index = -1 because of the 
        constructor design imo;
    */

    // Allocate that memory to nullptr
    all_point_objects[this_index] = nullptr;
    // I realised this was pointless, but also harmless so js let it be lol

    // Do compaction
    for (int i = this_index; i < total_point_count-1; i++) all_point_objects[i] = all_point_objects[i+1];
    
    // Reduce total point count
    total_point_count--;
}

// Copy constructor
Point::Point(const Point &other) {
    // implement deep copy
    this->x = other.x;
    this->y = other.y;
    this->alias = new string(*other.alias);

    all_point_objects[total_point_count] = this;
    total_point_count++;
}

void Point::show() const {
    this->doShowThis();
    cout<<"Show [" << *this->alias << "]: "<<"X: "<<this->x<<",Y: "<<this->y<<endl;
}

void Point::toggleShowThis() {
    this->showThis = !this->showThis;
}

void Point::setAlias(const string* alias) {
    *(this->alias) = *alias;
}

void Point::doShowThis() const {
    // printing `this' pointer can help in tracking how many point objects are active at a time
    if (this->showThis) {
        cout<< "[" << this << "]";
    }
}

// overload = operator, to do copy assignment operation
// e.g. p1 and p2 are two existing points. p1 = p2 should copy the content of p2 to p1. There should not be any memory leak.
//The operator should return a reference to p1 after copy assignment.
Point& Point::operator=(const Point& other)
{
    if (this == &other) return *this;
    delete this->alias;
    this->x = other.x;
    this->y = other.y;
    this->alias = new string(*other.alias);

    return *this;
}

// overload +, -, + double, - double, ++, --

Point Point::operator+(const Point& other) const
{
    double x = this->x + other.x;
    double y = this->y + other.y;
    string alias = *this->alias + "-add-" + *other.alias;

    Point result(alias, x, y);
    return result;
}

Point Point::operator-(const Point& other) const
{
    double x = this->x - other.x;
    double y = this->y - other.y;

    string alias = *this->alias + "-diff-" + *other.alias;
    Point result(alias, x, y);
    return result;
}

Point Point::operator+(double value) const
{
    double x = this->x + value;
    double y = this->y;
    string alias = *this->alias;
    Point result(alias, x, y);
    return result;
}

Point Point::operator-(double value) const
{
    double x = this->x - value;
    double y = this->y;
    string alias = *this->alias;
    Point result(alias, x, y);
    return result;
}

Point& Point::operator++()
{
    this->x += 1;
    this->y += 1;
    return *this;
}

Point& Point::operator--()
{
    this->x -= 1;
    this->y -= 1;
    return *this;
}

// overload == operator to check equality of Point objects that match in every field

bool Point::operator==(const Point& other) const
{
    if (this->x == other.x && this->y == other.y && *this->alias == *other.alias) return true;
    else return false;
}

// define setters
void Point::setX(const double x)
{
    this->x = x;
} 
void Point::setY(const double y) 
{
    this->y = y;
}

Point *Point::findByAlias(const string &alias) {
    for (int i = 0; i < total_point_count; i++) {
        if (*(all_point_objects[i]->alias) == alias) {
            return all_point_objects[i];
        }
    }
    return nullptr;
}

void Point::deleteAllPoints() {
    // fill-in to free the memory held by all the live points, pointed to by all_point_objects
    while(total_point_count > 0) delete all_point_objects[0];

    // delete all_points_objects[0] automatically calls the destructor that does compaction too as we defined it
}

int main()
{
    string alias1 = "p1";
    string alias2 = "p2";
    Point p1(alias1, 10.1, 10.01);
    Point p2(alias2, 20.1, 20.2);

    p1.show();
    p2.show();

    p1 = p1.operator+(p2);
    p1.show();
    p2.show();

    p1 = p1.operator-(p2);
    p1.show();
    p2.show();

    p2 = p2.operator+(5.5);
    p2.show();

    p2 = p2.operator-(3.0);
    p2.show();

    p1 = p1.operator--();
    p1.show();

    p2 = p2.operator++();
    p2.show();
}
