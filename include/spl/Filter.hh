#ifndef SPL_FILTER_HH
#define SPL_FILTER_HH

#include "2DSignal.hh"
#include "any.hh"

namespace spl
{
  template<typename E, typename SignalType>
  struct Filter : global::Any<E>
  {
    Filter(const SignalType& sig)
    : _sig(sig)
    , _res(sig.domain())
    {}

    void operator()(){
      __spl_impl(op_exec)();
    }

    const SignalType &res()
    { return _res; }

    protected:
    const SignalType &_sig;
    SignalType _res;
  };
}//!spl
  
#endif

