#include "3DSignal.hh"
#include <iostream>
#include "BoundMorpher.hh"
#include "Upsampler.hh"
#include <assert.h>

int main()
{
  typedef unsigned value_type;
  const unsigned cwidth = 3, cheight = 3;
  const unsigned sampling_number = 9;
  traits_domain_type_(spl::Signal2D<unsigned>) dom(cwidth, cheight);
  spl::Signal2D<value_type> test(dom);

  char arr_test [cheight][cwidth] = 
  {
    {0,0,0},
    {0,9,0},
    {0,0,0}
  };

  char arr_result [cheight*sampling_number][cwidth*sampling_number] =
  {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, 
{0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0}, 
{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0}, 
{0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0}, 
{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0}, 
{0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0}, 
{0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 6, 5, 4, 4, 3, 3, 2, 1, 1, 0}, 
{0, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 6, 6, 5, 4, 4, 3, 2, 1, 1, 0}, 
{0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 7, 6, 6, 5, 4, 3, 2, 2, 1, 0}, 
{0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 8, 7, 6, 5, 5, 4, 3, 2, 1, 0}, 
{0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 7, 6, 6, 5, 4, 3, 2, 2, 1, 0}, 
{0, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 6, 6, 5, 4, 4, 3, 2, 1, 1, 0}, 
{0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 6, 5, 4, 4, 3, 3, 2, 1, 1, 0}, 
{0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0}, 
{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0}, 
{0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0}, 
{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0}, 
{0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0}, 
{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, 
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} 
  };

  for_each_pixels(test, x, y)
    test(x,y) = arr_test[y][x];

  spl::Up2DSampler<unsigned, spl::BilinearInterpolation, sampling_number> sampling(test);
  sampling();
  for_each_pixels(sampling.res(),x,y)
    assert(sampling.res()(x,y) == arr_result[y][x]);

  spl::Down2DSampler<unsigned, 9> s(sampling.res());
  s();
  for_each_pixels(s.res(),x,y)
    assert(s.res()(x,y) == arr_test[y][x]);
  return 0;
}
