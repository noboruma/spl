# ifndef SPL_SPL_HISTOGRAM_HH
# define SPL_SPL_HISTOGRAM_HH

#include <limits>
#include <vector>
#include "2DSignal.hh"

  namespace spl{
    template<typename S>
    struct Histogram
    {
      Histogram(const S &sig)
      : _sig(sig)
      {}

      void operator()()
      {
        traits_iterator_type(S) it(_sig.domain());
        for_each_elements(it)
        {
          auto bin = std::find(_bins.begin(), _bins.end(), _sig[it]);
          if(bin != _bins.end())
            ++_res[bin];
          else
          {
            bins.push_back(_sig[it]);
            _res.push_back(1);
          }
        }
      }

      std::vector<unsigned> &res(){return _res;}
      std::vector<traits_value_type(S)> &bins(){return _bins;}

      private:
      const S &_sig;

      std::vector<traits_value_type(S)> _bins;
      std::vector<unsigned> _res;

    };
  }//!spl

# endif
