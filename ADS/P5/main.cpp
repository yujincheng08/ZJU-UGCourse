#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

size_t N,M,L;
const size_t MAX = 65;

unsigned F[MAX],H[MAX];
char T[MAX][MAX];
size_t Hash(char c);
void getLen();
bool test();
int main()
{
    ios::sync_with_stdio(false);
    cin>>N;
    char c; unsigned f;
    for(size_t i=0;i!=N;++i)
    {
        cin>>c>>f;
        F[Hash(c)]=H[i]=f;
    }
    getLen();
    cin>>M;
    while(M--)
    {
        if(test())
            cout<<"Yes"<<endl;
        else
            cout<<"No"<<endl;
    }
    return 0;
}

inline size_t Hash(char c)
{
    if(c>='0'&&c<='9')
        return c-'0';
    else if(c>='A'&&c<='Z')
        return c-'A'+10u;
    else if(c>='a'&&c<='z')
        return c-'a'+36u;
    else
        return 62u;
}

void getLen()
{
    make_heap(H,H+N,greater<unsigned>());
    for(size_t i=0;i<N-1;++i)
    {
        pop_heap(H,H+N-i,greater<unsigned>());
        pop_heap(H,H+N-i-1,greater<unsigned>());
        L+= (H[N-i-2]+=H[N-i-1]);
        push_heap(H,H+N-i-1,greater<unsigned>());
    }
}

bool test()
{
    char c; size_t l=0;
    for(size_t i = 0;i!=N;++i)
    {
        cin>>c>>T[i];
        l+= F[Hash(c)]*strlen(T[i]);
    }
    if(l!=L)
        return false;
    qsort(T,N,sizeof(T[0]),[](const void* a, const void *b)->int
    {return strlen((const char*)a)-strlen((const char*)b);});
    for(size_t i = 0;i!=N;++i)
        for(size_t j=i+1;j!=N;++j)
            if(strncmp(T[i],T[j],strlen(T[i]))==0)
                return false;
    return true;
}


//bool test()
//{
//    char c; size_t l=0;
//    for(size_t i = 0;i!=N;++i)
//    {
//        cin>>c>>T[i];
//        l+= F[Hash(c)]*strlen(T[i]);
//    }
//    if(l!=L)
//        return false;
//    for(size_t i = 0;i!=N;++i)
//    {
//        for(size_t j=i+1;j!=N;++j)
//        {
//            size_t a = strlen(T[i]);
//            size_t b = strlen(T[j]);
//            if(strncmp(T[i],T[j],a<b?a:b)==0)
//                return false;
//        }
//    }
//    return true;
//}

