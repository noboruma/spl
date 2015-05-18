#include "Upsampler.hh"

namespace spl
{
  namespace ct
  {

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L>
    Up2DSampler<V, interpolation, L>::
    Up2DSampler(const Signal2D<V>& sig)
    : _sig(sig)
    , _up_sig(nullptr)
    {
      traits_domain_type(Signal2D<V>) dom(_sig.domain());
      for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
        dom[i] += L*(dom[i]-1);
      _up_sig.reset(new Signal2D<V>(dom));
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L>
    void Up2DSampler<V, interpolation, L>::
    operator()()
    {
      interpolation<V,L> op_int(_sig);
      for_each_inner_pixels((*_up_sig), x, y, 1)
      (*_up_sig)(x,y) = op_int(x,y);
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L>
    Up2DSequenceSampler<V,interpolation,L>::
    Up2DSequenceSampler(const Signal3D<V>& sig)
    : _sig(sig)
      , _up_sig(nullptr)
    {
      traits_domain_type(Signal3D<V>) dom(_sig.domain());
      for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
        dom[i] += L*(dom[i]-1);
      _up_sig.reset(new Signal3D<V>(dom));
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L>
    void Up2DSequenceSampler<V,interpolation,L>::
    operator()()
    {
      const std::vector<Signal2D<V> > signal = 
      static_cast<std::vector<Signal2D<V> > >(_sig);

#pragma omp parallel for
      for(unsigned i=0; i < (*_up_sig).domain()[2]; ++i)
      {
        Up2DSampler<V,interpolation,L> s(signal[i]);
        s();
        const Signal2D<V> &res = s.res();
        for_each_pixels(res,x,y)
        (*_up_sig)(x,y,i) = res(x,y);
      }
    }
  }//!ct
  namespace rt
  {

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation>
    Up2DSampler<V, interpolation>::
    Up2DSampler(const Signal2D<V>& sig, unsigned L_)
    : _sig(sig)
    , _up_sig(nullptr)
    , L(L_)
    {
      traits_domain_type(Signal2D<V>) dom(_sig.domain());
      for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
        dom[i] += L*(dom[i]-1);
      _up_sig.reset(new Signal2D<V>(dom));
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation>
    void Up2DSampler<V, interpolation>::
    operator()()
    {
      interpolation<V,0> op_int(_sig, L);
      for_each_inner_pixels((*_up_sig), x, y, 1)
      (*_up_sig)(x,y) = op_int(x,y);
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation>
    Up2DSequenceSampler<V,interpolation>::
    Up2DSequenceSampler(const Signal3D<V>& sig, unsigned L_)
    : _sig(sig)
    , _up_sig(nullptr)
    , L(L_)
    {
      traits_domain_type(Signal3D<V>) dom(_sig.domain());
      for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
        dom[i] += L*(dom[i]-1);
      _up_sig.reset(new Signal3D<V>(dom));
    }

    // =========================================================================
    template<typename V, template<typename V, unsigned L> class interpolation>
    void Up2DSequenceSampler<V,interpolation>::
    operator()()
    {
      const std::vector<Signal2D<V> > signal = 
      static_cast<std::vector<Signal2D<V> > >(_sig);

#pragma omp parallel for
      for(unsigned i=0; i < (*_up_sig).domain()[2]; ++i)
      {
        Up2DSampler<V,interpolation> s(signal[i], L);
        s();
        const Signal2D<V> &res = s.res();
        for_each_pixels(res,x,y)
        (*_up_sig)(x,y,i) = res(x,y);
      }
    }
  }//!rt
}//!spl
