#include <iostream>
#include <string>
using namespace std;
#define MAX_BASKET 10

class Quote
{
    private: 
        string Isbn;

    protected:
        double salePrice;
    
    public:
        // Constructor
        Quote(string ISBN, double SP)
        {
            this->Isbn = ISBN;
            this->salePrice = SP;
        }

        // Getters
        string isbn() const
        {
            return this->Isbn;
        }

        virtual double net_price(size_t n)
        {
            return n * this->salePrice;
        }
};

class Bulk_quote : Quote
{
    int minQuantity;
    int discountPercentage;

    public:
        Bulk_quote(string ISBN, double SP, int minQty, int discount) : Quote(ISBN, SP)
        {
            this->minQuantity = minQty;
            this->discountPercentage = discount;
        }

        double net_price(size_t n)
        {
            if (n >= this->minQuantity) return (1 - 0.01*this->discountPercentage)*(n * this->salePrice);
            return n*this->salePrice;
        }

};

class Disc_quote : Quote
{
    double minPrice;
    int discountPercentage;

    public:
        Disc_quote(string ISBN, double SP, double minP, int discount) : Quote(ISBN, SP)
        {
            this->minPrice = minP;
            this->discountPercentage = discount;
        }

        double net_price(size_t n)
        {
            if (n*this->salePrice >= minPrice) return (1 - 0.01*this->discountPercentage)*(n * this->salePrice);
            return n * this->salePrice;
        }
};

class Basket
{
    const Quote* basket[MAX_BASKET];
    int basketSize = 0;

    public:
        // adds the quote in the array. Do not call it more than 10 times. If called, ignore it.
        void add_item(const Quote& q) 
        {
            if (basketSize >= MAX_BASKET) return;
            basket[basketSize] = q;
        } 
        
        // should print the total net prices of all the quotes in the basket
        double total_receipt() const 
        {
            double totalPrice = 0;
            for (int i = 0; i < basketSize; i++)
            {
                size_t n;
                cout << "Enter quantity of the book number " << basket[i]->isbn() << " purchased" << endl;
                cin >> n;
                totalPrice += basket[i]->net_price(n);
            }
            return totalPrice;
        }
};

int main()
{
    Basket b;
    Quote q1("970-111111", 120.0);
    b.add_item(q1);
    b.total_receipt();
}

