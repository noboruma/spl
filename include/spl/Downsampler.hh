#ifndef SPL_DOWNSAMPLER_HH
#define SPL_DOWNSAMPLER_HH

#include "2DSignal.hh"
#include "stdtools.hh"

namespace spl
{
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
      for_each_element(it)
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
        for_each_pixel(s.res(), x, y)
        (*_down_sig)(x,y,i) = s.res()(x,y);
      }
    }
  };
  template<typename V, bool Uniform>
  struct Up1DSampler;

  /*
     template<typename V>
     struct Up1DSampler<V, true>
     {
     Up1DSampler(const Signal1D<V> &in)
     : _sig(in)
     , _res(nb_point)
     {}

     void operator()
     {

  // Get min drivative
  double min_derivative = std::abs(_curve[1] - _curve[0]); // TODO : replace by max int
  for(unsigned i=1; i < _curve.length(); ++i)
  {
  std::cout << std::fabs(_curve[i] - _curve[i-1]) <<std::endl;
  min_derivative = std::min(min_derivative, std::fabs((double)_curve[i] - _curve[i-1]));
  }

  std::cout << "min =" << min_derivative << std::endl;

  // Oversample the curve :
  std::vector<double> over_sampled_curve;
  std::vector<double> over_sampled_time;
  double acc =0, j = 0;
  for(unsigned i=1; i < _curve.length(); ++i)
  {
  unsigned cnt=0;
  if(_curve[i-1] < _curve[i])
  {

  for(j= _curve[i-1]+acc; j < _curve[i]; j+= min_derivative)
  {
  over_sampled_curve.push_back(j);
  ++cnt;
  }
  acc =  j - _curve[i];
  }
  else
  {

  for(j= _curve[i-1]+acc; j > _curve[i]; j-= min_derivative)
  {
  over_sampled_curve.push_back(j);
  ++cnt;
  }
  acc = j - _curve[i];
  }


  for(unsigned j=0; j < cnt; ++j)
  over_sampled_time.push_back(i-1 + (double)j/cnt);
  }
  over_sampled_curve.push_back(_curve[_curve.length()-1]);
  over_sampled_time.push_back(_curve.length()-1);
  }
  private:
  const Signal1D<V> &_sig;
  const Signal1D<V> _res;
  };
  */



}// !spl

#endif
