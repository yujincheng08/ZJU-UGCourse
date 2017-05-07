#include <iostream>
#include "fraction.h"
using namespace std;
int main()
{
    Fraction b = 1/9_f;
    if(!b)
        cout<<"no"<<endl;
    Fraction a = 4/2_f;
    if(a)
        cout<<"yes"<<endl;
    Fraction c = a + b;
    cout << (a*2) <<endl;
    cout << b*3 <<endl;
    cout << c <<endl;
    return 0;
}
