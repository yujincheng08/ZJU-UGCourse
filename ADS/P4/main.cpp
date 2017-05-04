#include <iostream>
#include <cstring>
#include <functional>
using namespace std;
const int N = 10000+5;
const int MIN = -10001;
using nextFun = function<void(size_t&)>;
using cmpFun = function<bool(int,int)>;
int t[N];
int Max[N];
int Num[N];
int Up[N];
int Down[N];
void LIS(int (&peck)[N], size_t s, size_t e, nextFun next);


int main()
{
    size_t n;
    cin >>n;
    Num[0]=-2e4;
    for(size_t i=1;i<=n;++i)
        cin>>Num[i];
    LIS(Up,1,n+1,[](size_t &i){++i;});
    LIS(Down,n,0,[](size_t &i){--i;});
    int maxlen=0, index, len;
    for(size_t i=1;i<=n;++i)
    {
        if(Up[i]==1||Down[i]==1)
            continue;
        len = Up[i] + Down[i] - 1;
        if(len == maxlen && len!=0
                && abs(Up[i]-Down[i])<abs(Up[index]-Down[index]))
            index = i;
        else if(len > maxlen)
        {
            maxlen = len;
            index = i;
        }
    }
    if(maxlen)
        cout<<maxlen<<' '<<index<<' '<<Num[index]<<endl;
    else
        cout<<"No peak shape"<<endl;
    return 0;
}


size_t search(int (&p)[N], size_t &t, int &n)
{
    size_t s=0u, e=t, mid;
    mid = (s+e)>>1;
    while(s<e)
    {
        if((mid!=0&&p[mid]==0)||Num[p[mid]]>n)
            e = mid-1;
        else
            s = mid+1;
        mid = (s+e)>>1;
    }
    while(Num[p[s]]>=n||s>t) --s;
    return p[s];
}

void LIS(int (&p)[N], size_t s, size_t e,
         nextFun next)
{
    size_t maxlen = 1;
    for(size_t i=s,len=1;i!=e;next(i))
    {
        size_t j = search(t,maxlen,Num[i]);
        len = p[i] = p[j]+1;
        if(len>maxlen) maxlen = len;
        int &less = t[len];
        if(less==0||Num[less]>Num[i])
            less = i;
    }
    memset(t,0,++maxlen*sizeof(int));
}
