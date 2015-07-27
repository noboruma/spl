#include "2DSignal.hh"
#include "3DSignal.hh"
#include "SignalUtils.hh"
#include <iostream>
#include <assert.h>

int main()
{
  /////////////////////////
  // Extremum tests
  /////////////////////////
  { // 2D Signal
    typedef signed value_type;
    const unsigned cwidth = 3, cheight = 3;
    typedef spl::Signal2D<value_type> signal;
    traits_domain_type_(signal) dom(cwidth, cheight);
    signal test(dom);
    spl::Signal2D<bool> mask(dom);

    char arr_test [cheight][cwidth] = 
    {
      {0,1,1},
      {1,9,2},
      {0,2,0}
    };
    bool arr_mask [cheight][cwidth] = 
    {
      {false,true,true},
      {true,false,true},
      {false,true,true}
    };

    for_each_pixel(test, x, y)
    test(x,y) = arr_test[y][x];

    for_each_pixel(mask, x, y)
    mask(x,y) = arr_mask[y][x];

    spl::Extremum<signal, std::less> e_l(test);
    e_l();
    assert(e_l.res_point()[spl::axis::x] == 0);
    assert(e_l.res_point()[spl::axis::y] == 0);
    assert(e_l.res() == 0);
    spl::Extremum<signal, std::less> e_lm(test);
    e_lm(mask);
    assert(e_lm.res_point()[spl::axis::x] == 2);
    assert(e_lm.res_point()[spl::axis::y] == 2);
    assert(e_lm.res() == 0);

    spl::Extremum<signal, std::greater> e_g(test);
    e_g();
    assert(e_g.res_point()[spl::axis::x] == 1);
    assert(e_g.res_point()[spl::axis::y] == 1);
    assert(e_g.res() == 9);
    spl::Extremum<signal, std::greater> e_gm(test);
    e_gm(mask);
    assert(e_gm.res_point()[spl::axis::x] == 2);
    assert(e_gm.res_point()[spl::axis::y] == 1);
    assert(e_gm.res() == 2);
  }

  { // 3D Signal
    typedef signed value_type;
    const unsigned cwidth = 3, cheight = 3, cdepth=2;
    typedef spl::Signal3D<value_type> signal;
    traits_domain_type_(signal) dom(cwidth, cheight, cdepth);
    signal test(dom);
    mutate_(signal, bool) mask(dom);

    char arr_test [cdepth][cheight][cwidth] = 
    {
      {
        {0,1,1},
        {1,9,2},
        {0,2,0}
      },
      {

        {1,2,2},
        {2,4,8},
        {2,3,0}
      }
    };
    bool arr_mask [cdepth][cheight][cwidth] = 
    {
      {
        {0,1,1},
        {1,0,1},
        {0,1,0}
      },
      {
        {0,0,1},
        {0,0,1},
        {0,1,1}
      }
    };

    for_each_voxel(test, x, y, z)
    test(x,y,z) = arr_test[z][y][x];

    for_each_voxel(mask, x, y, z)
    mask(x,y,z) = arr_mask[z][y][x];

    spl::Extremum<signal, std::less> e_l(test);
    e_l();
    assert(e_l.res_point()[spl::axis::x] == 0);
    assert(e_l.res_point()[spl::axis::y] == 0);
    assert(e_l.res_point()[spl::axis::z] == 0);
    assert(e_l.res() == 0);
    spl::Extremum<signal, std::less> e_lm(test);
    e_lm(mask);
    assert(e_lm.res_point()[spl::axis::x] == 2);
    assert(e_lm.res_point()[spl::axis::y] == 2);
    assert(e_lm.res_point()[spl::axis::z] == 1);
    assert(e_lm.res() == 0);

    spl::Extremum<signal, std::greater> e_g(test);
    e_g();
    assert(e_g.res_point()[spl::axis::x] == 1);
    assert(e_g.res_point()[spl::axis::y] == 1);
    assert(e_g.res_point()[spl::axis::z] == 0);
    assert(e_g.res() == 9);
    spl::Extremum<signal, std::greater> e_gm(test);
    e_gm(mask);
    assert(e_gm.res_point()[spl::axis::x] == 2);
    assert(e_gm.res_point()[spl::axis::y] == 1);
    assert(e_gm.res_point()[spl::axis::z] == 1);
    assert(e_gm.res() == 8);
  }

  /////////////////////////
  // Polygon mask tests
  /////////////////////////
  {
    bool test [7][7] = {
      {0, 0, 0, 0, 0, 0, 0 },
      {0, 0, 0, 1, 0, 0, 0 },
      {0, 0, 0, 1, 1, 0, 0 },
      {0, 0, 0, 1, 1, 1, 0 },
      {0, 1, 1, 0, 0, 0, 0 },
      {0, 0, 1, 0, 0, 0, 0 },
      {0, 0, 0, 0, 0, 0, 0 },
    };

    std::list<spl::Point2D> l;
    l.push_back(spl::Point2D(3,0));
    l.push_back(spl::Point2D(6,3));
    l.push_back(spl::Point2D(0,3));
    l.push_back(spl::Point2D(3,6));

    spl::Domain<2> dom(7,7);
    spl::PolygonMask p(dom, l);
    p();
    int c = 0;
    for_each_pixel(p.res(), x, y)
     assert(p.res()(x,y) == test[y][x]);
  }
  return 0;
}
