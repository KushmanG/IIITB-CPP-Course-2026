#include <iostream>
using namespace std;

void orderStocks(int &first, int &second)
{
    if (first > second) 
    {
        int x = first;
        first = second;
        second = x;
    }
    else
    {
        first = first;
        second = second;
    }
}

int main()
{
    int firstStock, secondStock, adjustment, minStock;
    cin >> firstStock >> secondStock >> adjustment >> minStock;

    int stockRef = firstStock;
    stockRef += adjustment;
    if (stockRef < minStock) stockRef = minStock;

    orderStocks(stockRef, secondStock);

    cout << stockRef << " " << secondStock << endl;

}