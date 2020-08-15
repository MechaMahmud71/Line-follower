#include <bits/stdc++.h>

using namespace std;

int *ar;
int arsize;

int bsearch(int n)
{
    int left = 0;
    int right = arsize - 1;
    while(left <= right)
    {
        int mid = (left + right) >> 1;
        if(ar[mid] < n) left = mid + 1;
        else if(ar[mid] > n) right = mid - 1;
        else return mid;
    }
    return -1;
}

void shiftpos(int pos)
{
    for(int i = arsize - 1; i > pos; --i) {
        ar[i] = ar[i - 1];
    }
}

void insert_after_val(int value, int num_in)
{
    int pos = bsearch(value);
    if(pos == -1) cout << "Element Not Present" << endl;
    else
    {
        arsize += num_in;
        ar = (int *) realloc(ar, arsize * sizeof(int));
        for(int i = arsize ; i < arsize; ++i) ar[i] = -1;
        while(num_in--)
        {
            cout << "Insert data : ";
            int n;
            cin >> n;
            if(ar[pos] < n && ar[pos + 1] > n)
            {
                shiftpos(pos + 1);
                ar[pos + 1] = n;
            }
            else
            {
                cout << "Can't insert" << endl;
                return;
            }
        }
        for(int i = 0; i < arsize; ++i) {
            cout << ar[i] << " ";
        }
        cout << endl;
    }
}

int main(void)
{
    cout << "Enter array size : ";
    cin >> arsize;
    ar = new int[arsize];
    cout << "Enter the elements : ";
    for(int i = 0; i < arsize; ++i) cin >> ar[i];
    cout << "Enter the value : ";
    int value;
    cin >> value;
    cout << "Enter number of inputs : ";
    int num_in;
    cin >> num_in;
    insert_after_val(value, num_in);
}
