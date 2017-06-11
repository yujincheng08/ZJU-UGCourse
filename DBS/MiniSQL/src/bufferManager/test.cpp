#include "File.h"
#include "BufferManager.h"
#include <QCoreApplication>
#include <random>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);
    File &file = BufferManager::open("xxx");
    std::ios::sync_with_stdio(false);
    int x;
    std::srand (std::time(NULL));
    for(int i = 0; i<5; ++i)
    {
        int r = rand() % 1000000;
        //std::cout<<r<<std::endl;
        x = file.get<int>(4*r);
        if(x!=r)
        {
            x = file.get<int>(4*r);
            std::cout<<"sad"<<x<<"\t"<<r<<std::endl;
        }
    }
    std::cin>>x;
    for(int i = 0; i<1000000; ++i)
    {
        int r = rand() % 1000000;
        //std::cout<<r<<std::endl;
        x = file.get<int>(4*r);
        if(x!=r)
        {
            x = file.get<int>(4*r);
            std::cout<<"sad"<<x<<"\t"<<r<<std::endl;
        }
    }
    std::cout<<"done"<<std::endl;
    return a.exec();
}
