#include <iostream>     //For io
#include <cstring>      //For memset function
#include <functional>   //For std::function
using namespace std;
const int N = 10000+5;  //Max number of sequence
using nextFun = function<void(size_t&)>;
//Define for the direction of the iterator
int t[N];   //Temporary array to store
            //the minimum number of the maximum number
            //of all the sub-sequences
            //with the same longest length
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

//Search the maximum number smaller than n
//of all minimum numbers of all the
//maximum number of all the sub-sequences
//with length euqals to m using binary search
size_t search(size_t &m, int &n)
{
    //Define s as start position,
    //e as end position
    //mid as middle position
    size_t s=0u, e=m, mid;
    //When s<e, continue iteration
    while(s<e)
    {
        //Find the middle number between s and e
        mid = (s+e)>>1;
        //If t[mid]=0, it means it's never been set
        //Otherwise, compare the number with n
        if((mid!=0&&t[mid]==0)||Num[t[mid]]>n)
            //move end position
            e = mid-1;
        else
            //move start position
            s = mid+1;
    }
    //Eliminate elements that equals to n
    //and out of searching range
    while(Num[t[s]]>=n||s>m) --s;
    //Return its index in Num
    return t[s];
}

void LIS(int (&p)[N], size_t s, size_t e,
         nextFun next)
{
    //Initialize the maxlen to be 1
    size_t maxlen = 1;
    for(size_t i=s,len=1;i!=e;next(i))
    {
        //Search the previous longest sub-sequence
        size_t j = search(maxlen,Num[i]);
        //Update the length by the previous length
        len = p[i] = p[j]+1;
        //It's longer than the maxlen! Update maxlen
        if(len>maxlen) maxlen = len;
        //Get the index of the minimum number of all
        //the sub-sequence with length equals to len
        int &less = t[len];
        //If it's never set or bigger the the current one,
        //update it
        if(less==0||Num[less]>Num[i])
            less = i;
    }
    //Reset the temporary array.
    memset(t,0,++maxlen*sizeof(int));
}
