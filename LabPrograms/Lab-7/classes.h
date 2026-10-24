#ifndef ECOM
#define ECOM
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Exception classes
class ECommerceException : public exception
{
    string msg;
    public:
        ECommerceException(const string& m) : msg(m) {};
        const char* what() const noexcept override { return msg.c_str(); }
};

class InvalidCategoryException : public ECommerceException
{
    public:
    InvalidCategoryException(const string& m) : ECommerceException(m) {};
};

class ProductNotFoundException : public ECommerceException
{
    public:
    ProductNotFoundException(const string& m) : ECommerceException(m) {};
};

//Abstract class
class Product 
{
    private:
        static int uniq;
        int id;
        string name;
        double basePrice;
    
    protected:
        double discount;
        double tax;


    public:
        Product(string n, double BP);
        virtual ~Product() = 0;
        int getId() const;
        string getName() const;
        double getBP() const;
        double getFinalPrice() const;
        void view() const;

};
// Derived classes
class Grocery : public Product 
{
    public:
        Grocery(string n, double BP);
        ~Grocery();
};
class Electronics : public Product 
{
    public:
        Electronics(string n, double BP);
        ~Electronics();
};
class Clothing : public Product 
{
    public:
        Clothing(string n, double BP);
        ~Clothing();
};
class BabyProduct : public Product 
{
    public:
        BabyProduct(string n, double BP);
        ~BabyProduct();
};

// Other classes
class Catalog 
{
    private:
        vector<Product*> products;

    public:
        Catalog();
        ~Catalog();
        Catalog& operator+(Product* p); // appends p; no output; returns *this.
        Product* find(int id) const;    // matching product, or nullptr.
        void view() const;              // one line per product, insertion order.
};

class CartItem 
{
    private:
        const Product* product;
        int count;
        double totalPrice;

    public:
        CartItem(Product* p, int c);
        ~CartItem();
        CartItem& operator+(int n); 
        // adds n to count, recomputes totalPrice = count * product->getFinalPrice(), returns *this. This is how a repeat order of a product 
        // already in the cart is merged into its existing line.
        double getTotalPrice() const;
        const Product* getProduct() const;
        int getCount() const;
};

class Cart 
{
    private:
        vector<CartItem*> items;
        Catalog* catalog;
    public:
        Cart(Catalog* c);
        ~Cart();
        Cart& operator+(Product* p); 
        // adds one unit of p:
        // merges into an existing CartItem for the same product via CartItem::operator+(1),
        // or else appends a new CartItem with count 1. 
        // Returns *this.
        bool order(int id, int count);
        // resolves id to a Product* (Cart needs catalog access give it a reference/pointer at construction, or pass the catalog in). 
        // Throws ProductNotFoundException if unmatched; 
        // otherwise applies operator+ count times and returns true.
        void view() const; // one line per cart item.
        double checkout() const; // prints and returns the sum of getTotalPrice() over all items
};


#endif //ECOM