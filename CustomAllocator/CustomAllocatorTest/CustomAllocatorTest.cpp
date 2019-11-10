
#include "stdafx.h"

using namespace std;

struct A
{
  int a;
  int b;
  bool c;
  char r[48];

  A() = default;
  A(bool v) : a(5), b(v ? 1 : 0), c(v) {}
};

int main()
{
  A * pA = new A();
  pA->b++;
  cout << pA->b << endl;
  delete pA;

  A * pA2 = new A(true);
  pA2->b++;
  cout << pA2->b << endl;
  delete pA2;

  A * parrA = new A[5];
  parrA[2].c = true;
  delete[] parrA;

  const int NO_STR = 5;
  if (auto p = (string*)malloc(NO_STR * sizeof(string)))
  {
    for (int i = 0; i != NO_STR; ++i) // populate the array
    {
      void * placement = p + i;
      new(placement) string(4, (char)('a' + i));
    }

    for (int j = 0; j != NO_STR; ++j)
      cout << "p[" << j << "] == " << p[j] << '\n';

    for (int i = NO_STR; i != 0; --i) // clean up
      p[i - 1].~string();

    free(p);
  }

  return 0;
}