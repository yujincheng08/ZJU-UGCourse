#include <iostream>
#include "hlist.h"
#include <string>
using namespace std;

int main()
{
    HList a;
    const int s = 9;
    a << 13 <<1.2 << s << 's' << 4 << "Hello World" << "I love you" << false;
    a.append(16).append(14).append(true) << false;
    for(auto &i : a.typeList<int>())
        cout<<i<<endl;
    //cout << boolalpha;
    const HList &b = a;
    for(auto &i : b.typeList<int>())
        cout<<i<<endl;
    //auto x = b.previousType<int>();
    //cout<<a;
    return 0;
}
