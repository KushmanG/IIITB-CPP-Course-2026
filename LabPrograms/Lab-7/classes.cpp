#include <iostream>
#include <string>
#include <vector>
#include "classes.h"
using namespace std;

// Product
int Product::uniq = 0;

Product::Product(string n, double BP)
{
    this->name = n;
    this->basePrice = BP;
    this->id = uniq;
    uniq++;
}

Product::~Product() {};

int Product::getId() const { return id; }

string Product::getName() const { return name; }

double Product::getBP() const { return basePrice; }

double Product::getFinalPrice() const
{
    double bp = this->getBP();
    double t = this->tax;
    double d = this->discount;
    
    return bp + (bp * t) - (bp * d);
}

void Product::view() const
{
    string n = this->name;
    int i = this->id;
    double fp = this->getFinalPrice();
    cout << n << " " << i << " " << fp << endl;
}

// Inherited Classes
// Baby Products
BabyProduct::BabyProduct(string n, double BP) : Product(n, BP)
{
    this->discount = 0.05;
    this->tax = 0.05;
}    

BabyProduct::~BabyProduct() { }

// Electronics
Electronics::Electronics(string n, double BP) : Product(n, BP)
{
    this->discount = 0.12;
    this->tax = 0.18;
}    

Electronics::~Electronics() { }


// Clothing
Clothing::Clothing(string n, double BP) : Product(n, BP)
{
    this->discount = 0.0;
    this->tax = 0.18;
}    

Clothing::~Clothing() {};

// Grocery
Grocery::Grocery(string n, double BP) : Product(n, BP)
{
    this->discount = 0.0;
    this->tax = 0.0;
}    

Grocery::~Grocery() {};

// Catalog
Catalog::Catalog() {};

Catalog::~Catalog() { for (size_t i = 0; i < products.size(); i++) delete products[i]; }

Catalog& Catalog::operator+(Product* p) // appends p; no output; returns *this.
{
    products.push_back(p);
    return *this;
}

Product* Catalog::find(int id) const    // matching product, or nullptr.
{
    for (size_t i = 0; i < products.size(); i++) if (id == products[i]->getId()) return products[i];
    return nullptr;
}

void Catalog::view() const              // one line per product, insertion order.
{
    for (size_t i = 0; i < products.size(); i++) products[i]->view();
}

// CartItem
CartItem::CartItem(Product* p, int c)
{
    this->product = p;
    this->count = c;
    this->totalPrice = c * p->getFinalPrice();
}

CartItem::~CartItem() {};


// adds n to count, recomputes totalPrice = count * product->getFinalPrice(), returns *this. 
// This is how a repeat order of a product already in the cart is merged into its existing line.
CartItem& CartItem::operator+(int n)
{
    count += n;
    totalPrice = count * product->getFinalPrice();
    return *this;
}

double CartItem::getTotalPrice() const { return totalPrice; }
const Product* CartItem::getProduct() const { return product; }
int CartItem::getCount() const { return count; }

// Cart
Cart::Cart(Catalog* c) { this->catalog = c; } 

Cart::~Cart() { for (size_t i = 0; i < items.size(); i++) delete items[i]; }

// adds one unit of p:
// merges into an existing CartItem for the same product via CartItem::operator+(1),
// or else appends a new CartItem with count 1. 
// Returns *this.
Cart& Cart::operator+(Product* p)
{
    for (size_t i = 0; i < items.size(); i++) 
    {
        if (p == items[i]->getProduct()) 
        {
            items[i]->operator+(1);
            return *this;
        }
    }
    CartItem* item = new CartItem(p, 1);
    items.push_back(item);
    return *this;
}

// resolves id to a Product* (Cart needs catalog access give it a reference/pointer at construction, or 
// pass the catalog in). Throws ProductNotFoundException if unmatched; 
// otherwise applies operator+ count times and returns true.
bool Cart::order(int id, int count)
{
    Product* p = catalog->find(id);
    if (p == nullptr) throw ProductNotFoundException("no product with id " + to_string(id));

    for (int i = 0; i < count; i++) operator+(p);
    return true;
}

void Cart::view() const // one line per cart item.
{
    for (size_t i = 0; i < items.size(); i++) 
    {
        int c = items[i]->getCount();
        const Product* p = items[i]->getProduct();
        int ID = p->getId();
        string n = p->getName();
        double tp = items[i]->getTotalPrice();

        cout << c << " " << ID << " " << n << " " << tp << endl;
    }
}

double Cart::checkout() const // prints and returns the sum of getTotalPrice() over all items
{
    double sum = 0.0;
    for (size_t i = 0; i < items.size(); i++) sum += items[i]->getTotalPrice();
    cout << sum << endl;
    return sum;
}