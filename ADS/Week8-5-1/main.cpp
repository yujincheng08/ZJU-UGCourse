#include <iostream>
using namespace std;
const int N = 205;
const int L = 10005;
int DP[N][L]; //DP[i][j] means colors[1:i] in order[1:j]
int order[N];
int colors[L];
int main()
{
    ios::sync_with_stdio(false);
    int l,m,n,cnt=0;
    cin>>n>>m;
    for(int i=1;i<=m;++i)
        cin>>order[i];
    cin>>l;
    for(int i=1;i<=l;++i)
        cin>>colors[i];
    for(int j=1;j<=m;++j)
        for(int i=1;i<=l;++i)
        {
            if(DP[j-1][i-1]!=DP[j-1][i]) cnt=0;
            if(colors[i]==order[j]) cnt++;
            DP[j][i] = max(DP[j-1][i-1]+cnt,DP[j-1][i]);
        }
    cout<<DP[m][l]<<endl;
    return 0;
}
