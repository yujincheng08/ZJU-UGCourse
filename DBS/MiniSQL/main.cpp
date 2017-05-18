#include <iostream>
extern "C" {
#include "test.h"
}

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    testCreateTable();
    return 0;
}
