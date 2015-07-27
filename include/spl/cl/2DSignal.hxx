#include "2DSignal.hh"
#include "3DSignal.hh"
namespace spl
{
  namespace cl 
  {
    // =======================================================================
    template<typename V>
    Signal2D<V>::Signal2D(unsigned w, unsigned h, const ::cl::Device &dev)
    : parent(traits_domain_type(Signal2D<V>)(w,h))
      , CLSignal<V>(dev,parent::data(), w*h)
    {
      _data = new V* [h];
      for(unsigned y=0; y < h; ++y)
        _data[y] = &parent::data()[y*w];
    }

    // =======================================================================
    //template<typename V>
    //Signal2D<V>::Signal2D(traits_domain_type(Signal2D<V>) dom,
    //                      const spl::NDSignal<spl::Signal3D<V> > &p,
    //                      V * data)
    //: parent(dom, p)
    //{
    //  unsigned w = dom[0];
    //  unsigned h = dom[1];

    //  _data = new V* [h];
    //  for(unsigned y=0; y < h; ++y)
    //    _data[y] = &data[y*w];
    //}

    // =======================================================================
    //template<typename V>
    //Signal2D<V>::Signal2D(Domain<2> dom)
    //: parent(dom)
    //{
    //  unsigned w = dom[0];
    //  unsigned h = dom[1];

    //  _data = new V* [h];
    //  for(unsigned y=0; y < h; ++y)
    //    _data[y] = &parent::data()[y*w];
    //}

    // =======================================================================
    //template<typename V>
    //Signal2D<V>::Signal2D(const Signal2D<V> &s)
    //: parent(s)
    //{
    //  unsigned h = s.domain()[1];
    //  _data = new V* [h];
    //  for(unsigned y=0; y < h; ++y)
    //    _data[y] = s._data[y];
    //}

    // =======================================================================
    template<typename V>
    Signal2D<V>::~Signal2D()
    {
      delete [] _data;
    }
  } //!cl
}//!spl

