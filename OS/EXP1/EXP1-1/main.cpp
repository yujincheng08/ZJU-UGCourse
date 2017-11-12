#include "cross.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc > 1)
    Cross cross(argv[1]);
  else
    cerr << "Not enough arguments" << endl;
  return 0;
}
