#include "cross.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc > 1) // should have at least two arguments
    Cross cross(argv[1]); // pass to the cross
  else
    cerr << "Not enough arguments" << endl; // error if not enough arguments
  return 0;
}
