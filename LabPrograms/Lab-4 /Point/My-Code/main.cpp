#include <iostream>
#include <string>
#include "point.h"
using namespace std;

enum class Action {
// define the enums here
    CREATE_POINT, 
    DEEP_COPY_POINT, 
    ADD_POINT, 
    DIFF_POINT, 
    ADD_X_TO_POINT, 
    ADD_Y_TO_POINT, 
    COPY_ASSIGN_POINT, 
    TOGGLE_SHOW_THIS, 
    GET_POINT_COUNT, 
    SET_POINT_X, 
    SET_POINT_Y, 
    SET_POINT_ALIAS, 
    DO_PRE_INC, 
    DO_PRE_DEC, 
    DO_EXIT, 
    COUNT
};

const char* ActionNames[] = {
    // fill-in the Array of action names aligned with enum order
    "CREATE POINT", 
    "DEEP COPY POINT", 
    "ADD POINT", 
    "DIFF POINT", 
    "ADD X TO POINT", 
    "ADD Y TO POINT", 
    "COPY ASSIGN POINT", 
    "TOGGLE SHOW THIS", 
    "GET POINT COUNT", 
    "SET POINT X", 
    "SET POINT Y", 
    "SET POINT ALIAS", 
    "DO PRE INC", 
    "DO PRE DEC", 
    "DO EXIT"
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
Action printMenu(istream& in = cin, ostream& out = cout) {
    int choice;
    out << "===== Point Actions Menu =====\n";
    // print the menu
    for (int i = 0; i < sizeof(ActionNames)/sizeof(ActionNames[0]); i++)
    {
        out << i << ". " << ActionNames[i] << "\n";
    }
    out << "==============================\n";
    out << "Enter choice: ";
    in >> choice;
    // out the choice -- ActionName must be printed, not the int value
    out << "  " << ActionNames[choice] << endl;
    // return the choice, matching the return type
    Action action = static_cast<Action>(choice);
    return action;
}

// finds the pointer by alias. 
Point*  getPointOrNull(const string& alias) {
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
        case Action::CREATE_POINT:
        {
            out << "Enter: <X> <Y> <alias>" << endl;
            in >> px >> py >> alias;

            // create a point with the given details
            Point *point = new Point(alias, px, py);
            break;
        } 

        case Action::DEEP_COPY_POINT:
        {
            out << "Enter <alias>:" << endl;
            in >> alias;

            // find the point by alias, make one deep-copy of it using Point's copy constructor
            Point *point1 = getPointOrNull(alias);
            if (point1 == nullptr) break;

            Point* copy = new Point(*point1);
            break;
        }

        // Shared case block
        case Action::ADD_POINT:
        case Action::DIFF_POINT:
        case Action::COPY_ASSIGN_POINT:
        {
            out << "Enter <l_alias> <r_alias>:" << endl;
            in >> alias >> s_alias;
            // find both the points `alias' and `s_alias'
            Point *p1 = getPointOrNull(alias);
            Point *p2 = getPointOrNull(s_alias);
            if (p1 == nullptr || p2 == nullptr) break;
            
            if (actionItem == Action::ADD_POINT) *p1 = *p1 + *p2;     // do addition between two points, `alias' point should be the sum value
            else if (actionItem == Action::DIFF_POINT) *p1 = *p1 - *p2; // subtract two points, `alias' point should be the result value
            else *p1 = *p2;                               // copy the content of `s_alias' into `alias'
            break;
        }

        case Action::ADD_X_TO_POINT:
        case Action::ADD_Y_TO_POINT:
        {
            out << "Enter <alias> <value>" << endl;
            in >> alias >> px;
            // find point by `alias'
            Point *p3 = getPointOrNull(alias);
            if (p3 == nullptr) break;
            
            if (actionItem == Action::ADD_X_TO_POINT) *p3 = p3->operator+(px); // add px to the point
            else p3->setY(px);                                       // add px from the point
            break;
        }

        

        case Action::TOGGLE_SHOW_THIS:
        {
            out << "Enter <alias>:" << endl;
            in >> alias;
            // find point by `alias'
            Point *p4 = getPointOrNull(alias);
            if (p4 == nullptr) break;

            // toggle showThis flag of the point
            p4->toggleShowThis();
            break;
        }

        case Action::GET_POINT_COUNT:
        {
            out << Point::getPointCount() << endl;
            break;
        }

        case Action::SET_POINT_X:
        case Action::SET_POINT_Y:
        {
            out << "Enter <alias> <value>:" << endl;
            in >> alias >> px;
            // find point by `alias'
            Point *p5 = getPointOrNull(alias);
            if (p5 == nullptr) break;

            if (actionItem == Action::SET_POINT_X) p5->setX(px); // set x of point
            else p5->setY(px); // set y of point
            break;
        }

        case Action::SET_POINT_ALIAS:
        {
            out << "Enter <alias> <new-alias>:" << endl;
            in >> alias >> s_alias;
            // find point by `alias'
            Point *p6 = getPointOrNull(alias);
            if (p6 == nullptr) break;

            // set alias of the point as `s_alias'
            p6->setAlias(&s_alias);
            break;
        }

        case Action::DO_PRE_DEC:
        case Action::DO_PRE_INC:
        {
            out << "Enter <alias>:" << endl;
            in >> alias;
            // find point by `alias'
            Point *p7 = getPointOrNull(alias);
            if (p7 == nullptr) break;

            if (actionItem == Action::DO_PRE_INC) p7->operator++(); // do pre-increment ++ operation 
            else p7->operator--();              // do pre-decrement -- operation
            break;
        }

        default:
        {
            out << "Invalid command." << endl;
            break;
        }
    }
}

// infinite loop until user commands to exit
void runEventLoop(ostream& out=cout) {
    while (true) {
        Point::showAllPointObjects();
        Action actionItem = printMenu();
        if (actionItem == Action::DO_EXIT){
            Point::deleteAllPoints();
            out<<"Exit"<<endl;
            return;
        }
        doAction(actionItem);
    }
}

// calling the driver
int main() {
    runEventLoop();
}
