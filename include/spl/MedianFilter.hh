#ifndef SPL_MEDIAN_FILTER_HH
#define SPL_MEDIAN_FILTER_HH
#include "Filter.hh"
#include "2DSignal.hh"
namespace spl{

  template<typename V, bool sequence>
  struct MedianFilter : public Filter<MedianFilter<V,sequence>,Signal3D<V>>{

    typedef Filter<MedianFilter<V,sequence>,Signal3D<V>> parent;

    MedianFilter(const Signal3D<V> &sig)
    : parent(sig)
    {}

    void op_exec_impl()
    {
      const std::vector<Signal2D<V>> &arr(parent::_sig);
      #pragma omp parallel for
      for(unsigned z=0; z < arr.size(); ++z)
      {
        for_each_inner_pixel(arr[z], x, y, 1)
        {
          std::vector<V> values;
          for(signed i = -1; i <= 1; ++i)
            for(signed j = -1; j <= 1; ++j)
              values.push_back(arr[z]((signed)x+i,(signed)y+j));

          std::sort(values.begin(),values.end());
          parent::_res(x,y,z) = values[values.size()/2];
        }
      }
    }

  };

}//!spl

#endif

