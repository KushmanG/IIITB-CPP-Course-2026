
#include <iostream>
#include <string>
#include "point.h"

using namespace std;


int Point::total_point_count = 0;
Point* Point::all_point_objects[MAX_POINTS] = {nullptr};

int Point::getPointCount() {
    return total_point_count;
}

Point *Point::getPointObjects() {
    return *all_point_objects;
}

void Point::showAllPointObjects() {
    cout << "=========== POINTS REPO =============" << endl;
    for (int i = 0; i < total_point_count; i++) all_point_objects[i]->show();
    cout << "=========== POINTS REPO END =============" << endl;
}

Point::Point() {
    all_point_objects[total_point_count] = this;
    total_point_count++;
}

Point::Point(string &alias, double x, double y) {
    // fill in: show initialize the data members properly, should update the static data members of Point class properly
}

Point::~Point() {
    // delete in case there is heap memory in the data members before updating statics
    all_point_objects[total_point_count] = nullptr;
    total_point_count--;
}

Point::Point(const Point &other) {
    // implement deep copy
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


    // overload +, -, + double, - double, ++, --

    // overload == operator to check equality of Point objects that match in every field

    // define getters and setters

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
}
