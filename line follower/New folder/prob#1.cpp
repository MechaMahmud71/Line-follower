#include <bits/stdc++.h>

using namespace std;

int *ar;
int arsize;

int bsearchup(int n)
{
    int left = 0;
    int right = arsize - 1;
    int save = -1;
    while(left <= right)
    {
        int mid = (left + right) >> 1;
        if(ar[mid] < n) left = mid + 1;
        else if(ar[mid] > n) right = mid - 1;
        else
        {
            save = mid;
            right = mid - 1;
        }
    }
    return save;
}

int bsearchlow(int n)
{
    int left = 0;
    int right = arsize - 1;
    int save = -1;
    while(left <= right)
    {
        int mid = (left + right) >> 1;
        if(ar[mid] < n) left = mid + 1;
        else if(ar[mid] > n) right = mid - 1;
        else
        {
            save = mid;
            left = mid + 1;
        }
    }
    return save;
}

void frequency(int n)
{
    int ans = bsearchlow(n) - bsearchup(n) + 1;
    cout << ans << endl;
    //else cout << "Not Found" << endl;
}

int main(void)
{
    cout << "Enter array size : ";
    cin >> arsize;
    ar = new int[arsize];
    cout << "Enter the elements : ";
    for(int i = 0; i < arsize; ++i) cin >> ar[i];
    int val;
    cout << "Enter the value : ";
    cin >> val;
    frequency(val);
}
