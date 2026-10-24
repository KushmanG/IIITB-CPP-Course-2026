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


class NumArray 
{
    void bubbleSort(int array[10], int n = 10)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n-i-1; j++)
            {
                order(arr[j], arr[j+1]);
            }
        } 
    }

    public:
        int arr[10];
        void sort()
        {
            bubbleSort(arr);
        }

        const int* show()
        {
            for (int i = 0; i < 10; i++) cout << arr[i] << " ";
            cout << endl;
            return arr;
        }

        NumArray(int A[10])
        {
            for (int i = 0; i < 10; i++)
            {
                arr[i] = A[i];
            }
        }
};

int main()
{
    int A[10] = {2,1,3,0,5,4,7,8,6,9};
    NumArray n1(A);

    n1.sort();
    n1.show();
    return 0;
}