#include "2DSignal.hh"
#include <iostream>
#include <assert.h>
#include "io/io.hh"
int main()
{
  // 2D Signal
  {
    typedef signed value_type;
    const unsigned cwidth = 3, cheight = 3;
    traits_domain_type_(spl::Signal2D<value_type>) dom(cwidth, cheight);
    spl::Signal2D<value_type> test(dom);

    char arr_test [cheight][cwidth] = {
      {0,1,1},
      {1,9,3},
      {0,2,0}
    };

    for_each_pixel(test, x, y)
      test(x,y) = arr_test[y][x];

    spl::io::raw::save("./tmp_io_raw", test);

    for_each_pixel(test, x, y)
      test(x,y) = 0;

    spl::io::raw::load("./tmp_io_raw", test);

    for_each_pixel(test, x, y)
      assert(test(x,y) == arr_test[y][x]);

    spl::io::csv::save("./tmp_io_csv", test);
    for_each_pixel(test, x, y)
      test(x,y) = 0;

    spl::io::csv::load("./tmp_io_csv", test);

    for_each_pixel(test, x, y)
      assert(test(x,y) == arr_test[y][x]);
  }
  return 0;
}
