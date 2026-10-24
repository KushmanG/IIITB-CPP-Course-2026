#ifndef SALESDATA_H
#define SALESDATA_H

#include <iostream>
#include <string>
using namespace std;

class Sales_data
{
    string bookNo;
    double revenue;
    unsigned int units_sold;

    double avg_price() const;

    public:
        //Constructors
        // Default Constructor
        Sales_data();
        // ISBN Constructor
        Sales_data(string ISBN);
        // 3-Param constructor
        Sales_data(string ISBN, unsigned int quantity, double price);

        Sales_data& combine(const Sales_data &other);

        friend ostream& operator<<(ostream& os, const Sales_data& s);

        // Gets the ISBN of a book
        string isbn() const;
    };
    
Sales_data add(const Sales_data &lhs, const Sales_data &rhs);

#endif // SALESDATA_H
