#include <iostream>

using namespace std;
const int Max = 100 + 5;
int Area[Max][Max];
int DP[Max][Max];
int R, C;
int dp(int x, int y);

int main()
{
    ios::sync_with_stdio(false);
    cin>>R>>C;
    for(int i=0; i!=R; ++i)
        for(int j=0; j!=C; ++j)
            cin>>Area[i][j];
    int c = 0, m = 0;
    for(int i =0; i!=R; ++i)
    {
        for(int j=0; j!=C; ++j)
        {
            c = dp(i,j);
            m = max(m,c);
        }
    }
    cout<< m << endl;
    return 0;
}

int dp(int x, int y)
{
    static int dx[4]={0,0,-1,1};
    static int dy[4]={-1,1,0,0};
    int &c = DP[x][y];
    if(c)
        return c;
    for(int d = 0; d<4; ++d)
    {
        int newx = x + dx[d];
        int newy = y + dy[d];
        if(newx >= 0 && newy >= 0 && newx < R && newy < C && Area[x][y] > Area[newx][newy])
            c = max(c, dp(newx,newy));
    }
    ++c;
    return c;
}
