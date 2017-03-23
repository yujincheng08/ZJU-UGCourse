#include <iostream>
#include <cfloat>
#include <cmath>
using namespace std;
double ln(int x);
double harmonic(int x);
int main()
{
    int upper;
    cin>>upper;
//    double result = 1 + upper;
//    for(int i = 2; i < upper; ++i)
//        result/=i;
//    result/=2;
    cout<<ln(upper)<<endl;
    return 0;
}

double harmonic(int x)
{
    static double Euler = 0.57721566490153286060651209;
    return Euler + ln(x) + 1.0/2/x;
}

double ln(int x)
{
    if(x==0)
        return 0xFF800000L;
    double result = --x;
    int tmp = x;
    x = -x;
    for(int i = 2;i>0;++i)
    {
        tmp *= x;
        double factor = tmp/(double)i;
        result += factor;
        if(fabs(factor)<=0.000001)
            break;

    }
    return result;
}
