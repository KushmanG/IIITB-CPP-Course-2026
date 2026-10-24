#ifndef CPP_POINT_H
#define CPP_POINT_H

#include <iostream>
#include <string>
using namespace std;

#define MAX_POINTS 10



class Point {
    // core data members of Point class
    double x;
    double y;
    string *alias;

    /* Methods for DEBUGGING.. No EDIT ZONE */
    bool showThis = false;
    void doShowThis() const;

    // static members to keep track of live point objects
    static int total_point_count;
    static Point* all_point_objects[MAX_POINTS];

public:
    // constructors
    Point();
    Point(string& alias, double x, double y);

    // destructor
    ~Point();

    // copy-constructor
    Point(const Point &other);

    // overloaded operators
    Point& operator=(const Point& other);

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;

    Point operator+(double value) const;
    Point operator-(double value) const;

    Point& operator++();
    Point& operator--();

    bool operator==(const Point& other) const;

    // prints details of the point object
    void show() const;

    // set/reset the flag to print `this'
    void toggleShowThis();

    // setters
    void setX(const double x);
    void setY(const double y);
    void setAlias(const string* alias);

    // static members
    static int getPointCount();
    static Point* getPointObjects();
    static void showAllPointObjects();
    static Point* findByAlias(const string& alias);
    static void deleteAllPoints();
};

#endif // CPP_POINT_H