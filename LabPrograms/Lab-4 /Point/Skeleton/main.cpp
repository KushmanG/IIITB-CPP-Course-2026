#include <iostream>
#include <string>
#include "point.h"
using namespace std;

enum class Action {
// define the enums here
};

const char* ActionNames[] = {
    // fill-in the Array of action names aligned with enum order
};

// Operator<< uses array instead of switch
ostream& operator<<(ostream& os, const Action& choice) {
    int idx = static_cast<int>(choice);
    if (idx >= 0 && idx < static_cast<int>(Action::COUNT))
        os << ActionNames[idx];
    else
        os << "UNKNOWN";
    return os;
}

// Prints menu using the same array
<fill-in the return type> printMenu(istream& in = cin, ostream& out = cout) {
    int choice;
    out << "===== Point Actions Menu =====\n";
    // print the menu
    out << "==============================\n";
    out << "Enter choice: ";
    in >> choice;
    // out the choice -- ActionName must be printed, not the int value
    // return the choice, matching the return type
}

// finds the pointer by alias. 
Point* getPointOrNull(const string& alias) {
    Point* p = Point::findByAlias(alias);
    if (p == nullptr) cout << alias << " does not exist" << endl;
    return p;
}

// switch-case for all the commands, based on the input choice
void doAction(Action actionItem, istream& in = cin, ostream& out = cout) {
    
    // variables to cin user params to execute the commands
    double px, py;
    string alias, s_alias;
    Point *p, *oldp, *otherp;

    switch (actionItem) {
        case //CREATE_POINT:
            out << "Enter: <X> <Y> <alias>" << endl;
            in >> px >> py >> alias;
            // create a point with the given details
            break;

        case //DEEP_COPY_POINT:
            out << "Enter <alias>:" << endl;
            in >> alias;
            // find the point by alias, make one deep-copy of it using Point's copy constructor
            break;

        case //ADD_POINT:
        case //DIFF_POINT:
        case //COPY_ASSIGN_POINT:
            out << "Enter <l_alias> <r_alias>:" << endl;
            in >> alias >> s_alias;
            // find both the points `alias' and `s_alias'
            
            if (//ADD_POINT)     // do addition between two points, `alias' point should be the sum value
            else if (//DIFF_POINT) // subtract two points, `alias' point should be the result value
            else                                      // copy the content of `s_alias' into `alias'
            break;

        case //ADD_X_TO_POINT:
        case //SUB_X_TO_POINT:
            out << "Enter <alias> <value>" << endl;
            in >> alias >> px;
            // find point by `alias'
            if (//ADD_X_TO_POINT) // add px to the point
            else                  // subtract px from the point
            break;

        case //TOGGLE_SHOW_THIS:
            out << "Enter <alias>:" << endl;
            in >> alias;
            // find point by `alias'
            // toggle showThis flag of the point
            break;

        case //GET_POINT_COUNT:
            out << Point::getPointCount() << endl;
            break;

        case //SET_POINT_X:
        case //SET_POINT_Y:
            out << "Enter <alias> <value>:" << endl;
            in >> alias >> px;
            // find point by `alias'
            if (//SET_POINT_X) // set x of point
            else // set y of point
            break;

        case //SET_POINT_ALIAS:
            out << "Enter <alias> <new-alias>:" << endl;
            in >> alias >> s_alias;
            // find point by `alias'
            // set alias of the point as `s_alias'
            break;

        case //DO_PRE_DEC:
        case //DO_PRE_INC:
            out << "Enter <alias>:" << endl;
            in >> alias;
            // find point by `alias'
            if (//DO_PRE_INC) // do pre-increment ++ operation 
            else              // do pre-decrement -- operation
            break;

        default:
            out << "Invalid command." << endl;
            break;
    }
}

// infinite loop until user commands to exit
void runEventLoop(ostream& out=cout) {
    while (true) {
        Point::showAllPointObjects();
        Action actionItem = printMenu();
        if (//DO_EXIT){
            Point::deleteAllPoints();
            out<<"Exit"<<endl;
            return;
        }
        doAction(actionItem);
    }
}

// initialize the static class variables. 
// It cannot be done inside the class, it cannot be done inside any function, it has to done before main() is called.
// Even though they are private, for initialization, we are allowed for the following syntax.
int Point::total_point_count = 0;
Point* Point::all_point_objects[MAX_POINTS] = {nullptr};

// calling the driver
int main() {
    runEventLoop();
}
