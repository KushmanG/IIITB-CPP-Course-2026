#include <iostream>
#include <string>
#include "Sales_data.h"

double Sales_data::avg_price() const
{
    if (this->units_sold == 0) return 0.0;
    return this->revenue/this->units_sold;
}

Sales_data::Sales_data()
{
    this->bookNo = "";
    this->revenue = 0.0;
    this->units_sold = 0;
}
// ISBN Constructor
Sales_data::Sales_data(string ISBN)
{
    this->bookNo = ISBN;
    this->revenue = 0.0;
    this->units_sold = 0;
}
// 3-Param constructor
Sales_data::Sales_data(string ISBN, unsigned int quantity, double price)
{
    this->bookNo = ISBN;
    this->revenue = quantity*price;
    this->units_sold = quantity;
}

Sales_data& Sales_data::combine(const Sales_data &other)
{
    if (this->bookNo != other.bookNo) return *this;
    this->revenue += other.revenue;
    this->units_sold += other.units_sold;
    return *this;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data result = lhs;
    result.combine(rhs);
    return result;
}

ostream& operator<<(ostream& os, const Sales_data& s) {
    os << s.bookNo << " " << s.units_sold << " " << s.revenue << " " << s.avg_price() << "\n";
    return os;
}

        
// Gets the ISBN of a book
string Sales_data::isbn() const
{
    return this->bookNo;
}

int main()
{
    // 1. Construct objects using every constructor
    Sales_data d1;               // default constructor
    Sales_data d2("1234");          // ISBN-only constructor
    Sales_data d3("1230", 3, 20.0); // three-parameter constructor

    cout << endl;
    cout << d1;   // zero-sales: avg price must print as 0
    cout << d2;      // zero-sales: avg price must print as 0
    cout << d3;      // 1230 3 60 20

    const int RECORD_COUNT = 5;
    Sales_data records[RECORD_COUNT];

    cout << "\nEnter " << RECORD_COUNT << " records as: <ISBN> <quantity> <price>\n";
    for (int i = 0; i < RECORD_COUNT; ++i)
    {
        string isbn;
        unsigned int qty;
        double price;
        cin >> isbn >> qty >> price;
        records[i] = Sales_data(isbn, qty, price);
    }

    cout << endl;
    for (int i = 0; i < RECORD_COUNT; ++i) cout << records[i] << endl;

    // 3. Combine compatible transactions.
    //    Start from a copy of the first record and fold in every later
    //    record that shares its ISBN.
    Sales_data total = records[0];
    for (int i = 1; i < RECORD_COUNT; ++i) total.combine(records[i]);   // combine() ignores mismatched ISBNs

    cout << endl;
    cout << total;

    // Show that combine() does nothing when the ISBNs differ.
    Sales_data d4("44121431", 2, 10.0);
    Sales_data d5("34252354", 5, 4.0);
    Sales_data d6 = d4;
    d4.combine(d5);
    cout << endl;
    cout << "Before: " << d6;
    cout << "After : " << d4;
    cout << "Nothing happened cz ISBNs are different :)" << endl;

    // 4. Verify average price manually for at least one example.
    //    978-2222222222: 4 units * 25.0 => revenue 100.0, avg price 100/4 = 25.0
    Sales_data d7("1908743", 4, 25.0);
    cout << endl;
    cout << d7;
    cout << "Expected avg price = 100.0 / 4 = 25\n";

    // 5. avg_price() must return 0, not divide by 0.
    Sales_data d8("6969");
    cout << endl;
    cout << d7;   // 6969 0 0 0

    // 6. Show that add() does not modify its arguments.
    Sales_data lhs("6767", 2, 15.0);
    Sales_data rhs("6767", 3, 15.0);
    cout << endl;
    cout << "lhs before: " << lhs;
    cout << "rhs before: " << rhs;
    Sales_data sum = add(lhs, rhs);
    cout << "result    : " << sum;   // 6767 5 75 15
    cout << "lhs after : " << lhs;   // unchanged
    cout << "rhs after : " << rhs;   // unchanged

    return 0;
}