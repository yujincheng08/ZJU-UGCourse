#include <stdio.h>

#define Max 105
int Area[Max][Max];
int DP[Max][Max];
int R, C;
int dp(int x, int y);

// function to get the greater
int max(int x, int y)
{
    return x>y?x:y;
}

int main()
{
    scanf("%d%d",&R,&C);
    for(int i=0; i!=R; ++i)
        for(int j=0; j!=C; ++j)
            scanf("%d", &Area[i][j]); // Input all data
    int m = 0;
    for(int i =0; i!=R; ++i)
        for(int j=0; j!=C; ++j)
            m = max(m,dp(i,j)); // try all start positions
    printf("%d",m);
    return 0;
}

int dp(int x, int y)
{
    static int dx[4]={0,0,-1,1};
    static int dy[4]={-1,1,0,0};  // delta of co-ordinates
    if(DP[x][y])
        return DP[x][y]; // Already calculated!
    for(int d = 0; d<4; ++d)
    {
        int newx = x + dx[d];
        int newy = y + dy[d]; // new co-ordinates
        if(newx >= 0 && newy >= 0 && newx < R
                && newy < C && Area[x][y] > Area[newx][newy])
            // Not out of bound and have lower height
            DP[x][y] = max(DP[x][y], dp(newx,newy));
    }
    ++DP[x][y]; // Plus one of the result
    return DP[x][y];
}
