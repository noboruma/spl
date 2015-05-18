#include "2DSignal.hh"
#include <iostream>
#include <assert.h>
#include "Histogram.hh"

int main()
{
  typedef signed value_type;
  const unsigned cwidth = 3, cheight = 3;
  traits_domain_type_(spl::Signal2D<value_type>) dom(cwidth, cheight);
  spl::Signal2D<value_type> test(dom);

  char arr_test [cheight][cwidth] = 
  {
    {0,1,1},
    {1,9,2},
    {0,2,0}
  };

  for_each_pixels(test, x, y)
    test(x,y) = arr_test[y][x];

  spl::Histogram<spl::Signal2D<value_type>> h(test);

  h();

  assert(h.res()[0] == 3);
  assert(h.res()[1] == 3);
  assert(h.res()[2] == 2);
  assert(h.res()[9] == 1);
  assert(h.res()[3] == 0);

  return 0;
}
