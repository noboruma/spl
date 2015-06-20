# ifndef SPL_SPL_HISTOGRAM_HH
# define SPL_SPL_HISTOGRAM_HH

#include <vector>
#include <unordered_map>
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
        for_each_element(it)
          ++_res[_sig[it]];
      }

      std::unordered_map<traits_value_type(S), unsigned> _res;
      decltype(_res) &res(){return _res;}

      private:
      const S &_sig;

    };
  }//!spl

# endif
