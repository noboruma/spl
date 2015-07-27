#include "1DSignal.hh"

  namespace spl
  {

    // =====================================================================
    template<typename V>
    Signal1D<V>::Signal1D(unsigned w)
    : parent(traits_domain_type(Signal1D<V>)(w))
    {
      _data = &parent::data()[0];
    }

    template<typename V>
    template<typename ... Args>
    Signal1D<V>::Signal1D(unsigned w, Args ... args)
    : parent(traits_domain_type(Signal1D<V>)(w), args...)
    {
      _data = &parent::data()[0];
    }

    // =====================================================================
    template<typename V>
    Signal1D<V>::Signal1D(Domain<1> dom)
    : parent(dom)
    {
      unsigned w = dom[0];
      _data = &parent::data()[0];
    }

    // =====================================================================
    template<typename V>
    template<typename ... Args>
    Signal1D<V>::Signal1D(Domain<1> dom, Args ... args)
    : parent(dom, args...)
    {
      _data = &parent::data()[0];
    }

    // =====================================================================
    template<typename V>
    Signal1D<V>::Signal1D(const Signal1D<V> &sig)
    : parent(sig)
    {
      _data = &parent::data()[0];
    }

  }//!spl
