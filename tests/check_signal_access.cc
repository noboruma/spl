#include "spl/2DSignal.hh"
#include "spl/3DSignal.hh"
#include <iostream>
#include <assert.h>
#include "spl/BoundMorpher.hh"
#include "spl/io/io.hh"
int main()
{
  // 2D Signal
  {

    typedef signed value_type;
    const unsigned cwidth = 3, cheight = 3;
    traits_domain_type_(spl::Signal2D<value_type>) dom(cwidth, cheight);
    spl::Signal2D<value_type> test(dom);

    char arr_test [cheight][cwidth] = 
    {
      {0,1,1},
      {1,9,3},
      {0,2,0}
    };

    for_each_pixel(test, x, y)
    test(x,y) = arr_test[y][x];

    for_each_pixel(test, x, y)
    std::cout<<test(x,y)<<std::endl;

    for_each_pixel_par(test, x, y)
    test(x,y) = arr_test[y][x];

    for_each_pixel(test, x, y)
    assert(test(x,y) == arr_test[y][x]);

    for_each_pixel(test, x, y)
    assert(test[spl::Point2D(x,y)] == arr_test[y][x]);

    traits_iterator_type_(spl::Signal2D<value_type>) it(dom);
    for_each_element(it)
    assert(test(it[0], it[1]) == test[it]);

    // BoundMorpher
    {
      const spl::BoundMorpher<spl::Signal2D<value_type> > slice_bm(test, spl::Point2D(0,0), spl::Point2D(1,1));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal2D<value_type>>) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::y]][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 4);
      assert(sum == 11);
    }
    {
      const spl::BoundMorpher<spl::Signal2D<value_type> > slice_bm(test, spl::Point2D(0,1), spl::Point2D(2,1));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal2D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::y]+1][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 3);
      assert(sum == 13);
    }
    {
      const spl::BoundMorpher<spl::Signal2D<value_type> > slice_bm(test, spl::Point2D(1,0), spl::Point2D(1,2));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal2D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::y]][it_slice[spl::axis::x]+1]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 3);
      assert(sum == 12);
    }
  }
  {
    // 3D Signal
    typedef signed value_type;
    const unsigned cwidth = 3, cheight = 3, cdepth = 2;
    traits_domain_type_(spl::Signal3D<value_type>) dom(cwidth, cheight, cdepth);
    spl::Signal3D<value_type> test(dom);

    char arr_test [cdepth][cheight][cwidth] = 
    {
      {
        {0,1,1},
        {1,9,2},
        {0,2,0}
      },
      {
        {10,11,11},
        {11,19,12},
        {10,12,10}
      }
    };

    for_each_voxel(test, x, y, z)
    test(x,y,z) = arr_test[z][y][x];

    for_each_voxel_par(test, x, y, z)
    test(x,y,z) = arr_test[z][y][x];

    for_each_voxel(test, x, y, z)
    assert(test(x,y,z) == arr_test[z][y][x]);

    for_each_voxel(test, x, y, z)
    assert(test[spl::Point3D(x,y,z)] == arr_test[z][y][x]);

    traits_iterator_type_(spl::Signal3D<value_type>) it(dom);
    for_each_element(it)
    assert(test(it[0], it[1], it[2]) == test[it]);

    // BoundMorpher
    {
      const spl::BoundMorpher<spl::Signal3D<value_type> > slice_bm(test, spl::Point3D(0,0,0), spl::Point3D(1,1,1));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal3D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::z]][it_slice[spl::axis::y]][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 8);
      assert(sum == 62);
    }
    {
      const spl::BoundMorpher<spl::Signal3D<value_type> > slice_bm(test, spl::Point3D(0,0,0), spl::Point3D(0,0,1));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal3D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::z]][it_slice[spl::axis::y]][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 2);
      assert(sum == 10);
    }
    {
      const spl::BoundMorpher<spl::Signal3D<value_type> > slice_bm(test, spl::Point3D(0,0,0), spl::Point3D(2,0,0));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal3D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::z]][it_slice[spl::axis::y]][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 3);
      assert(sum == 2);
    }
    {
      const spl::BoundMorpher<spl::Signal3D<value_type> > slice_bm(test, spl::Point3D(0,0,0), spl::Point3D(0,2,0));

      traits_iterator_type_(spl::BoundMorpher<spl::Signal3D<value_type> >) it_slice(slice_bm.domain());
      unsigned cpt=0;
      unsigned sum =0;
      for_each_element(it_slice)
      {
        assert(slice_bm[it_slice] == arr_test[it_slice[spl::axis::z]][it_slice[spl::axis::y]][it_slice[spl::axis::x]]);
        sum += slice_bm[it_slice];
        ++cpt;
      }
      assert(cpt == 3);
      assert(sum == 1);
    }
  }
  return 0;
}
