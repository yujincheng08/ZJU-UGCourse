#include <iostream>
#include "fraction.h"
using namespace std;

int main()
{
    Fraction b = 0/87654321_f;
    if(!b)
        cout<<"no"<<endl;
    Fraction a = 1/1111111111_f;
    if(a)
        cout<<"yes"<<endl;
    Fraction c = a + b;
    cout << a <<endl;
    cout << b <<endl;
    cout << c <<endl;
    return 0;
}
