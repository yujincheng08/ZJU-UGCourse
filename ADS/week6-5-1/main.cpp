#include <iostream>
#include <ios>
#include <cstring>
using namespace std;
char problems[9][21];
int baseTime[9];
int debugTime[9];
bool mark[9];
void go();
void initial();
int totalTime;
int bestNum;
int realTime;
int H,N,t0;
int stack[9];
int top = 0;
int finalStack[9];
int main()
{
    ios::sync_with_stdio(false);
    while(true)
    {
        cin>>H;
        if(H<0) break;
        cin>>N>>t0;
        for(int i=0;i<N;++i)
        {
            cin>>problems[i]>>baseTime[i]>>debugTime[i];
            mark[i]=false;
        }
        go();
    }
    return 0;
}

void choose(int thisTime, int thisRealTime)
{
    for(int i = 0; i<N ;++i)
    {
        if(!mark[i])
        {
            mark[i]=true;
            stack[top++]=i;
            int submitTime = thisRealTime + baseTime[i];
            int debugTimes = (submitTime-1)/60;
            int newRealTime = submitTime + debugTime[i] * debugTimes;
            int newTime = thisTime + newRealTime + debugTimes *20;
            if(newRealTime <= 60* H)
                choose(newTime,newRealTime);
            mark[i]=false;
            --top;
        }
    }
    if(top > bestNum || (top==bestNum && thisTime < totalTime))
    {
        bestNum = top;
        totalTime = thisTime;
        memcpy(finalStack,stack,sizeof(int)*(top));
    }
}

void go()
{
    top = 0;
    realTime =t0;
    totalTime = t0;
    bestNum = 0;
    choose(0,t0);
    cout<<"Total Time = "<<totalTime<<endl;
    for(int i=0;i<bestNum;++i)
        cout<<problems[finalStack[i]]<<endl;
}

