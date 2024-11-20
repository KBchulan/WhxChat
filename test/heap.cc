#include <iostream>

using namespace std;

int *allocateInteger()
{
    int *p = new int(42);
    return p;
}

int main()
{
    int *ref = allocateInteger();
    cout << *ref << endl;
    return 0;
}