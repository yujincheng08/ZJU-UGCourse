#include <iostream>
#include "fraction.h"
using namespace std;

int main()
{
    Fraction b(1,2);
    int a = 1;
    Fraction c;
    cout << (a<b) <<endl;
    cout << (a<=b) <<endl;
    cout << (a>b) <<endl;
    cout << (a>=b) <<endl;
    cout << 2*a <<endl;
    cout << (double)a <<endl;
    return 0;
}
