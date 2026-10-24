#include <iostream>
#include <string>
using namespace std;

void order(string& one, string& other)
{
    if (one > other)
    {
        string x = one;
        one = other;
        other = x;
    }
}


class StringArray 
{
    void bubbleSort(string array[10])
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 9-i; j++)
            {
                order(arr[j], arr[j+1]);
            }
        } 
    }

    public:
        string arr[10];
        void sort()
        {
            bubbleSort(arr);
        }

        const string* show()
        {
            for (int i = 0; i < 10; i++) cout << arr[i] << " ";
            cout << endl;
            return arr;
        }

        StringArray(string A[10])
        {
            for (int i = 0; i < 10; i++)
            {
                arr[i] = A[i];
            }
        }
};

int main()
{
    string A[10] = {"A","B","D","C","R","E","X","T","Q","P"};
    StringArray s1(A);

    s1.sort();
    s1.show();
    return 0;
}