#include <iostream>
#include <algorithm>
#include<iomanip>
using namespace std;

double C, D, Da, M, P;
size_t N;
double greedy(double s, double e, double r);

struct Station
{
    double d;
    double p;
    bool s;
    inline Station(){}
    inline Station(const double &D):d(D){}
} S[505];

inline bool operator <(const Station &a, const Station &b)
{
    return a.d < b.d;
}

int main()
{
    ios::sync_with_stdio(false);
    cin>>C>>D>>Da>>N;
    M = Da * C;
    for(size_t i=0;i<N;++i)
        cin>>S[i].p >> S[i].d;
    sort(S,S+N);
    double r = greedy(0,D,0);
    cout<<fixed<<setprecision(2);
    if(r == D)
        cout<<P<<endl;
    else
        cout<<"The maximum travel distance = "<<r<<endl;
    return 0;
}

Station *findMin(double s, double e)
{
    Station *r = lower_bound(S,S+N,Station(s));
    Station *l = lower_bound(r,S+N,Station(e));
    while(r->s && r<S+N) ++r;
    for(Station *i=r;i<l;++i)
        if(!i->s && ((i->p < r->p )||(i->p==r->p&&i->d<r->d)))
            r = i;
    return r;
}

/*
Station *findMin(double s, double e)
{
    size_t r = 0;
    for(;(S[r].d<s || S[r].d>=e || S[r].s) && r<N;++r);
    for(size_t i=r+1;i<N;++i)
    {
        if(S[i].d>=s && S[i].d<e && !S[i].s)
        {
            if(S[i].p < S[r].p||(S[i].p==S[r].p &&S[i].d<S[r].d))
                r = i;
        }
    }
    return S+r;
}
 */

double greedy(double s, double e, double r)
{
    Station *m = findMin(s,e);
    if(m==S+N) return r;
    const double &d = m->d;
    const double &p = m->p;
    m->s = true;
    if(d>r)
    {
        r = greedy(s,d,r);
        if(r<d) return r;
    }
    if(d+M>=e)
    {
        P += (e-r)*p/Da;
        return e;
    }
    else
    {
        P += p*(d+M-r)/Da;
        return greedy(d,e,d+M);
    }
}
