#include <iostream>
#include <limits>
#include <gaol/gaol>

using namespace std;
using namespace gaol;

int main(int argc, char **argv)
{
  interval I(numeric_limits<double>::denorm_min());

  cout << I << endl;
  cout << I.midpoint() << endl;

  return EXIT_SUCCESS;
}








































