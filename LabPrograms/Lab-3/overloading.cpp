#include <iostream>
using namespace std;

void order(int& one, int& other)
{
    if (one > other)
    {
        int x = one;
        one = other;
        other = x;
    }
}
int min(int a1, int b1)
{
    if (a1 > b1) return b1;
    else return a1;
}
double min(double a2, double b2)
{
    if (a2 > b2) return b2;
    else return a2;
}
int min(int arr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len-i-1; j++)
        {
            int one = arr[j];
            int other = arr[j+1];
            if (one > other)
            {
                int x = one;
                one = other;
                other = x;
            }
        }
    } 
    return arr[0];
}


int main()
{
    int a1 = 10;
    int b1 = 20;

    double a2 = 1.23;
    double b2 = 2.47;

    int arr[] = {5,3,7,9,10,56};
    int len = 6;

    int x1 = min(a1,b1);
    double x3 = min(a2,b2);
    int x5 = min(arr,len);

    cout << x1 << " " << x3 << " " << x5 << endl;
    return 0;
}