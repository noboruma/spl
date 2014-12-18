#include "3DSignal.hh"
#include <iostream>
#include "BoundMorpher.hh"
#include "Mean.hh"
#include <assert.h>

int main()
{
  typedef int value_type;
  const unsigned cdepth = 2,
                 cwidth = 3,
                 cheight = 3;
  char tab[cdepth][cheight][cwidth] = 
  {
    {
      {1, 2 ,3},
      {4, 5 ,6},
      {7 ,8 ,9},
    },
    {
      {11, 12 ,13},
      {14, 15 ,16},
      {17 ,18 ,19},
    }
  };

  // Fill Signal3D with data
  spl::Signal3D<value_type> sig(cwidth,cheight,cdepth);
  for(unsigned z=0; z < cdepth; ++z)
    for(unsigned y=0; y < cheight; ++y)
      for(unsigned x=0; x < cwidth; ++x)
        sig(x,y,z) = tab[z][y][x];


  //////////////////////////////////
  // Mean on axis
  //////////////////////////////////
  {
    // X axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,value_type, 0, true > m(sig);
      m();
      const spl::Signal2D<value_type>& res = m.res();

      for_each_pixels(res, x, y)
      {
        int mean = 0;
        for(unsigned i=0; i < cwidth; ++i)
          mean += tab[y][x][i];
        mean /= cwidth;

        assert(res(x,y) == mean);
      }
    }

    // Y axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>, value_type, 1, true > m(sig);
      m();
      const spl::Signal2D<value_type>& res = m.res();

      for_each_pixels(res, x, y)
      {
        int mean = 0;
        for(unsigned i=0; i < cheight; ++i)
          mean += tab[y][i][x];
        mean /= cheight;

        assert(res(x,y) == mean);
      }
    }

    // Z axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,value_type, 2, true > m(sig);
      m();
      const spl::Signal2D<value_type>& res = m.res();

      for_each_pixels(res, x, y)
      {
        int mean = 0;
        for(unsigned i=0; i < cdepth; ++i)
          mean += tab[i][y][x];
        mean /= cdepth;
        assert(res(x,y) == mean);
      }
    }

  }

  //////////////////////////////////
  // Mean of slices along axis
  //////////////////////////////////
  {

    //X axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,double, 0, false > m(sig);
      m();
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cwidth; ++i)
      {
        double mean=0;
        for(unsigned z=0; z < cdepth; ++z)
          for(unsigned y=0; y < cheight; ++y)
            mean += tab[z][y][i];
        mean /= (cheight*cdepth);
        assert(res(i) == mean);
      }
    }

    //Y axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,double, 1, false > m(sig);
      m();
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cheight; ++i)
      {
        double mean=0;
        for(unsigned z=0; z < cdepth; ++z)
          for(unsigned x=0; x < cwidth; ++x)
            mean += tab[z][i][x];
        mean /= (cdepth*cwidth);
        assert(res(i) == mean);
      }
    }

    //Z axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,double, 2, false > m(sig);
      m();
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cdepth; ++i)
      {
        double mean=0;
        for(unsigned y=0; y < cheight; ++y)
          for(unsigned x=0; x < cwidth; ++x)
            mean += tab[i][y][x];
        mean /= (cheight*cwidth);
        assert(res(i) == mean);
      }
    }
  }

  //////////////////////////////////
  // Mean of slices along axis using mask
  //////////////////////////////////
  {
    bool depth_arr_mask[cheight][cwidth] = 
    {
        {true, true ,false},
        {true, false ,true},
        {false ,false ,true},
    };
    spl::Signal2D<bool> dmask(cheight, cwidth);
    for(unsigned y=0; y < cheight; ++y)
      for(unsigned x=0; x < cwidth; ++x)
        dmask(x,y) = depth_arr_mask[y][x];

    bool width_arr_mask[cdepth][cwidth] = 
    {
        {true, true ,false},
        {true, false ,true},
    };
    spl::Signal2D<bool> wmask(cheight, cdepth);
    for(unsigned z=0; z < cdepth; ++z)
      for(unsigned y=0; y < cheight; ++y)
        wmask(y,z) = width_arr_mask[z][y];

    bool height_arr_mask[cdepth][cwidth] = 
    {
        {true, true ,false},
        {true, false ,true},
    };

    spl::Signal2D<bool> hmask(cwidth,cdepth);
    for(unsigned y=0; y < cdepth; ++y)
      for(unsigned x=0; x < cwidth; ++x)
        hmask(x,y) = height_arr_mask[y][x];

    //X axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>, double, 0, false > m(sig);
      m(wmask);
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cwidth; ++i)
      {
        volatile double mean = 0;
        unsigned cpt = 0;
        for(unsigned z=0; z < cdepth; ++z)
          for(unsigned y=0; y < cheight; ++y)
            if(width_arr_mask[z][y])
            {
              mean += tab[z][y][i];
              ++cpt;
            }
        mean /= cpt;
        assert(res(i) == mean);
      }
    }

    //Y axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,double, 1, false > m(sig);
      m(hmask);
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cheight; ++i)
      {
        volatile double mean=0;
        unsigned cpt=0;
        for(unsigned z=0; z < cdepth; ++z)
          for(unsigned x=0; x < cwidth; ++x)
            if(height_arr_mask[z][x])
            {
              mean += tab[z][i][x];
              ++cpt;
            }
        mean /= cpt;
        assert(res(i) == mean);
      }
    }
    //Z axis
    {
      spl::algo::Mean<spl::Signal3D<value_type>,double, 2, false > m(sig);
      m(dmask);
      const spl::Signal1D<double>& res = m.res();

      for(unsigned i=0; i < cdepth; ++i)
      {
        volatile double mean=0;
        unsigned cpt=0;
        for(unsigned y=0; y < cheight; ++y)
          for(unsigned x=0; x < cwidth; ++x)
            if(depth_arr_mask[y][x])
            {
              mean += tab[i][y][x];
              ++cpt;
            }
        mean /= cpt;
        assert(res(i) == mean);
      }
    }
  }

  return 0;
}
//END_TEST
