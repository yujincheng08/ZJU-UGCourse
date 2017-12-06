#include "cross.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc > 1) // Should have at least two arguments
    Cross cross(argv[1]); // Pass the second to the cross
  else
    cerr << "Not enough arguments" << endl; // Error if arguments not enough
  return 0;
}
