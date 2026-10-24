#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "classes.h"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <command file>." << endl;
        return 1;
    }

    //Input and Output Streams
    ifstream infile(argv[1]);
    ofstream outfile("error.log");

    // Initializing all Objects
    Catalog* catalog = new Catalog();
    Cart* cart = new Cart(catalog);

    // Command Loop
    string command;
    while (infile >> command)
    {
        string fullCommand = command;
        try 
        {
            if (command == "exit") {
                break;
            } else if (command == "add") {
                string type, name;
                double basePrice;
                infile >> type >> name >> basePrice;

                fullCommand += " " + type + " " + name + " " + to_string(basePrice);
                if (type == "grocery") {
                    Grocery* g = new Grocery(name, basePrice);
                    catalog->operator+(g);
                } else if (type == "electronic") {
                    Electronics* e = new Electronics(name, basePrice); 
                    catalog->operator+(e);
                } else if (type == "clothing") {
                    Clothing* c = new Clothing(name, basePrice);
                    catalog->operator+(c);
                } else if (type == "babyproduct") {
                    BabyProduct* b = new BabyProduct(name, basePrice);
                    catalog->operator+(b);
                } else { throw InvalidCategoryException("unrecognized product category '" + type + "'"); }
            } else if (command == "catalog") {
                catalog->view();
            } else if (command == "order") {
                int id, count;
                infile >> id >> count;
                fullCommand += " " + to_string(id) + " " + to_string(count);
                cart->order(id, count);
            } else if (command == "cart") {
                cart->view();
            } else if (command == "checkout") {
                cart->checkout();
            } else { cerr << "Error: Invalid Command" << endl; }
        }

        catch (const ECommerceException& e) { outfile << "ERROR: " << e.what() << " | command: " << fullCommand << endl; }
    }

    delete cart;
    delete catalog;
    return 0;
}