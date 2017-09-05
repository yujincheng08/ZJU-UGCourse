#include "app.h"

int main()
{
    if(!initApp()) //initialize program
    {
        endApp(); //if failed to initialize, end the program and return 1 to indicate error
        return 1;
    }
    return exec(); //if success initializing, then enter message loop in exec();
}
