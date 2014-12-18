#ifndef SPL_UPSAMPLER_HH
#define SPL_UPSAMPLER_HH

#include <2DSignal.hh>
#include "stdtools.hh"

  namespace spl
  {
    template<typename V, unsigned L>
    struct BilinearInterpolation
    {
      const Signal2D<V>& _sig;
      BilinearInterpolation(const Signal2D<V>& sig)
      : _sig(sig)
      {}

      V operator()(unsigned x, unsigned y)
      {
        double x1 = x/(L+2);
        double y1 = y/(L+2);
        double x2 = x1+1;
        double y2 = y1+1;

        double f11 = _sig(x1, y1);
        double f12 = _sig(x1, y2);
        double f21 = _sig(x2, y1);
        double f22 = _sig(x2, y2);

        x1 *= (L+1);
        y1 *= (L+1);
        x2 *= (L+1);
        y2 *= (L+1);

        double res = /*(double)((1/((x2-x1)*(y2-y1))) */ (f11*(x2 - x)*(y2 - y)
                                               + f21*(x - x1)*(y2 - y)  
                                               + f12*(x2 - x)*(y - y1) 
                                               + f22*(x - x1)*(y - y1) );
        res /= ((x2-x1)*(y2-y1));
        return std::round(res);
      }

    };

    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L=1>
    struct Up2DSampler
    {
      const Signal2D<V>& _sig;
      std::unique_ptr<Signal2D<V> > _up_sig;

      const Signal2D<V>& res(){ return (*_up_sig); }

      Up2DSampler(const Signal2D<V>& sig)
      : _sig(sig)
      , _up_sig(nullptr)
      {
        traits_domain_type(Signal2D<V>) dom(_sig.domain());
        for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
          dom[i] += L*(dom[i]-1);
        _up_sig.reset(new Signal2D<V>(dom));
      }

      void operator()()
      {
        interpolation<V,L> op_int(_sig);
        traits_iterator_type(Signal2D<V>) it((*_up_sig).domain());
        for_each_elements(it)
         (*_up_sig)[it] = op_int(it[0],it[1]);
      }
    };

    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L=1>
    struct Up2DSequenceSampler
    {
      const Signal3D<V> &_sig;
      std::unique_ptr<Signal3D<V> > _up_sig;

      const Signal3D<V> &res(){ return (*_up_sig); }

      Up2DSequenceSampler(const Signal3D<V>& sig)
      : _sig(sig)
      , _up_sig(nullptr)
      {
        traits_domain_type(Signal3D<V>) dom(_sig.domain());
        for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
          dom[i] += L*(dom[i]-1);
        _up_sig = new Signal3D<V>(dom);
      }

      void operator()()
      {
        const std::vector<Signal2D<V> > &signal = static_cast<std::vector<Signal2D<V> > >(_sig);

        /*for(unsigned i=0; i < (*_up_sig).domain()[2]; ++i)
        {
          Up2DSampler<V,interpolation,L> s(signal[i]);
          s();
          const Signal2D<V> &res = s.res();
          for_each_pixels(res,x,y)
            (*_up_sig)(x,y,i) = res(x,y);
        }*/
        std::cerr<<"Done "<<signal.size()<<std::endl;
      }
    };

    template<typename V, unsigned L=1>
    struct Down2DSampler
    {
      const Signal2D<V>& _sig;
      std::unique_ptr<Signal2D<V> > _down_sig;

      const Signal2D<V>& res(){ return (*_down_sig); }

      Down2DSampler(const Signal2D<V>& sig)
      : _sig(sig)
      , _down_sig(nullptr)
      {
        traits_domain_type(Signal2D<V>) dom(_sig.domain());
        for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
          dom[i] %= L;
        _down_sig.reset(new Signal2D<V>(dom));
      }

      void operator()()
      {
        traits_iterator_type(Signal2D<V>) it((*_down_sig).domain());
        for_each_elements(it)
         (*_down_sig)[it] = _sig[Point2D(it[0]*(L+1), it[1]*(L+1))];
      }
    };

    template<typename V,unsigned L=1>
    struct Down2DSequenceSampler
    {
      const Signal3D<V> &_sig;
      std::unique_ptr<Signal3D<V> > _down_sig;

      const Signal3D<V> &res(){ return (*_down_sig); }

      Down2DSequenceSampler(const Signal3D<V>& sig)
      : _sig(sig)
      , _down_sig(nullptr)
      {
        traits_domain_type(Signal3D<V>) dom(_sig.domain());
        for(unsigned i=0; i < traits_domain_dim(Signal2D<V>); ++i)
          dom[i] += L*(dom[i]-1);
        _down_sig.reset(new Signal3D<V>(dom));
      }

      void operator()()
      {
        std::vector<Signal2D<V> > signal(_sig);

        for(unsigned i=0; i < _sig.domain()[2]; ++i)
        {
          Down2DSampler<V,L> s(signal[i]);
          s();
          for_each_pixels(s.res(), x, y)
            (*_down_sig)(x,y,i) = s.res()(x,y);
        }
      }
    };


  }//!spl

#endif
