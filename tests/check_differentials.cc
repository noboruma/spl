#include "2DSignal.hh"
#include <iostream>
#include <assert.h>
#include "Differentials.hh"
#include "io/io.hh"

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

  char grad_x[cheight][cwidth];
  char grad_y[cheight][cwidth];

  for(size_t y=0; y < cheight; ++y)
    for(size_t x=0; x < cwidth; ++x)
      if(x != cwidth -1)
        grad_x[y][x] = arr_test[y][x+1] - arr_test[y][x];
      else //border
        grad_x[y][x] = 0;

  for(size_t x=0; x < cwidth; ++x)
    for(size_t y=0; y < cheight; ++y)
      if(y != cheight -1)
        grad_y[y][x] = arr_test[y+1][x] - arr_test[y][x];
      else // border
        grad_y[y][x] = 0;


  for_each_pixel(test, x, y)
    test(x,y) = arr_test[y][x];

  spl::Signal1D<spl::Signal2D<value_type>> gradients = spl::differentials::gradient(test);

  for_each_pixel(gradients(0), x, y)
    assert(gradients(0)(x,y) == grad_x[y][x]);

  for_each_pixel(gradients(1), x, y)
    assert(gradients(1)(x,y) == grad_y[y][x]);

  return 0;
}
