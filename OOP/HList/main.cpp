#include <iostream>
#include "hlist.h"
#include <string>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    HList a;
    const int s = 9;
    a <<1.2 << s << 's' << 4 << "Hello World" << "I love you" << false;
    a.pushBack(16).pushBack(14).pushBack(true) << false;
    a.pushFront(13);
    auto p = a.find(4);
    a.insert(1.4,p);
    p = a.find(16);
    a.earse(p);
    HList x;
    x<<333<<444<<"111"<<false;
    a.merge(x);
    cout << boolalpha;
    cout<<"List:"<<endl;
    for(auto i = a.begin(); i!= a.end(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"First element to int:"<<endl;
    cout<<"\t"<<a.begin().toValue<int>()<<endl;
    cout<<"Last element to bool:"<<endl;
    cout<<"\t"<<a.rbegin().toValue<bool>()<<endl;
    a.reverse();
    cout<<"After reverse: (Reverse output):"<<endl;
    for(auto i = a.rbegin(); i!= a.rend(); ++i)
        cout<<"\t"<<i<<endl;
    const HList &b = a;
    cout<<"Output all elements constantly:"<<endl;
    for(auto i = b.begin(); i!=b.end(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"Reverse output all elements constantly:"<<endl;
    for(auto i = b.rbegin(); i!=b.rend(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"First element to bool:"<<endl;
    cout<<"\t"<<b.begin().toValue<bool>()<<endl;
    cout<<"Last element to int:"<<endl;
    cout<<"\t"<<b.rbegin().toValue<int>()<<endl;
    a.popBack();
    a.popFront();
    cout<<"After pop back and front all the int:"<<endl;
    for(auto i = a.begin<int>(); i != a.end<int>(); ++i)
        cout<<"\t"<<(int)i<<endl;
    cout<<"Reverse output all int:"<<endl;
    for(auto i = a.rbegin<int>(); i!=a.rend<int>(); ++i)
        cout<<"\t"<<(int)i<<endl;
    cout<<"Output all int:"<<endl;
    for(auto i = b.begin<bool>(); i!=b.end<bool>(); ++i)
        cout<<"\t"<<(bool)i<<endl;
    cout<<"Reverse output all int:"<<endl;
    for(auto i = b.rbegin<bool>(); i!=b.rend<bool>(); ++i)
        cout<<"\t"<<(bool)i<<endl;
    auto c = a.typeList<int>();
    auto d = b.typeList<bool>();
    cout<<"Output all int using foreach:"<<endl;
    for(auto &i : c)
        cout<<"\t"<<i<<endl;
    cout<<"Output all bool using foreach:"<<endl;
    for(auto &i : d)
        cout<<"\t"<<i<<endl;
    cout<<"Output all int using for"<<endl;
    for(auto i = c.begin(); i!= c.end(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"Reverse output all int using for"<<endl;
    for(auto i = c.rbegin(); i!= c.rend(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"Output all bool using for"<<endl;
    for(auto i = d.begin(); i!= c.end(); ++i)
        cout<<"\t"<<i<<endl;
    cout<<"Reverse output all bool using for"<<endl;
    for(auto i = d.rbegin(); i!=d.rend(); ++i)
        cout<<"\t"<<i<<endl;
    return 0;
}
