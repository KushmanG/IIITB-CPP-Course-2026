#include <iostream>
#include <string>
using namespace std;

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
        double getFinalPrice();
        void view() const;
};

class Grocery : public Product
{
    protected:
        double tax;
        double discount;
};

class Clothing : public Product
{
    protected:
        double tax;
        double discount;
};

class Electronics : public Product
{
    protected:
        double tax;
        double discount;
};

class BabyProduct : public Product
{
    protected:
        double tax;
        double discount;
};

class Catalog
{
    private:    
        vector<Product> products;
    
    public:
        
};

class Cart
{

};

class CartItem
{

};