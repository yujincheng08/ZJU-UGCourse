#include <iostream>     //For io
#include <cstring>      //For memset function
#include <functional>   //For std::function
using namespace std;
const int N = 10000+5;  //Max number of sequence
using nextFun = function<void(size_t&)>;
//Define for the direction of the iterator=
int Num[N]; //Store the original sequence
int Up[N];  //Store the result of the longest
            //increasing sub-sequence
int Down[N];//Store the result of the longest
            //decreasing sub-sequence
//Subscript i of the above two arrays
//means the length of the longest
//sub-sequence start with the i-th number
void LIS(int (&peck)[N], size_t s, size_t e, nextFun next);
//Longest increase sub-sequence of array peck,
//indexed from s to e with the direction function next

int main()
{
    //Close synchronizing with stdio
    //to speed up cin and cout
    ios::sync_with_stdio(false);
    //Define and input the length of the original sequence
    size_t n;
    cin >>n;
    //Set the 0 element to be the minimum as sentinel
    Num[0]=-2e4;
    for(size_t i=1;i<=n;++i)
        cin>>Num[i]; //Input all numbers
    //Calculate the increasing sub-sequence
    LIS(Up,1,n+1,[](size_t &i){++i;});
    //Calculate the decreasing sub-sequence
    LIS(Down,n,0,[](size_t &i){--i;});
    //Calculate the maxlen of the peck sub-sequence,
    //with peck's index
    //len is used to store the temporary length
    int maxlen=0, index, len;
    //Enumerate the peck's position
    for(size_t i=1;i<=n;++i)
    {
        //Up[i] and Down[i] must bigger than 1
        //because the sub-sequences with length 1
        //only contain the peck itself, which violates
        //the definition
        if(Up[i]==1||Down[i]==1)
            continue;
        //Calculate the length of the sub-sequence
        //whose Peck indexed i.
        len = Up[i] + Down[i] - 1;
        //If the len is the same with the max one,
        //select the better one with lower defference
        if(len == maxlen && len!=0
                && abs(Up[i]-Down[i])
                    <abs(Up[index]-Down[index]))
            index = i;
        //If the len is bigger then the max one,
        //let it be the new max one
        else if(len > maxlen)
        {
            maxlen = len;
            index = i;
        }
    }
    //maxlen not equals to 0 means we find one
    if(maxlen)
        cout<<maxlen<<' '<<index<<' '<<Num[index]<<endl;
    //Otherwise, No peak shape
    else
        cout<<"No peak shape"<<endl;
    return 0;
}

void LIS(int (&p)[N], size_t s, size_t e,
         nextFun next)
{
    for(size_t i=s,len=1;i!=e;next(i))
    {
        size_t m = 0;
        for(size_t j=s;j!=i;next(j))
        {
            if(Num[j]<Num[i]&&(p[j]>p[m]||(p[j]==p[m]&&Num[j]<Num[m])))
                m = j;
        }
        p[i] = p[m]+1;
    }
}
