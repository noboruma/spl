# ifndef SPL_SPL_HISTOGRAM_HH
# define SPL_SPL_HISTOGRAM_HH

#include <limits>
#include <vector>
#include "2DSignal.hh"

  namespace spl{
    template<typename S, const unsigned max =std::numeric_limits<traits_value_type(S)>::max(), const unsigned min = -std::numeric_limits<traits_value_type(S)>::min() >
    struct Histogram
    {
      Histogram(const S &sig)
      : _sig(sig)
      {}

      void operator()()
      {
        /*if (std::numeric_limits<traits_value_type(S)>::is_signed
          &&  !std::numeric_limits<traits_value_type(S)>::is_integer)
          {
          std::cerr<<"unspported"<<std::endl;
          return;
          }*/
        _res.resize(max + min + 1, 0);

        traits_iterator_type(S) it(_sig.domain());
        for_each_elements(it)
        ++_res[_sig[it]];
      }

      std::vector<unsigned> &res(){return _res;}

      private:
      const S &_sig;
      std::vector<unsigned> _res;

    };
  }//!spl

# endif
