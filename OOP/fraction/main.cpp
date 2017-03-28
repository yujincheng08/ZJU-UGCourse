#include <iostream>
#include "fraction.h"
using namespace std;

int main()
{
    Fraction b = 1235678/87654321_f;
    Fraction a = 1/1111111111_f;
    Fraction c = a + b;
    cout << a <<endl;
    cout << b <<endl;
    cout << c <<endl;
    return 0;
}
