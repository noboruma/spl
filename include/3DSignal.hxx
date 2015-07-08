#include "3DSignal.hh"

  namespace spl
  {
    // =======================================================================
    template<typename V>
    Signal3D<V>::Signal3D(unsigned w, unsigned h, unsigned d)
    : parent(traits_domain_type(Signal3D<V>)(w,h,d))
    {
      _data = new V** [d];
      for(unsigned z=0; z < d; ++z)
      {
        _data[z] = new V*[h]; 
        for(unsigned y=0; y < h; ++y)
          _data[z][y] = &parent::data()[y*w + z*w*h];
      }
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::Signal3D(Domain<3> dom)
    : parent(dom)
    {
      unsigned w = dom[0];
      unsigned h = dom[1];
      unsigned d = dom[2];
      _data = new V** [d];
      for(unsigned z=0; z < d; ++z)
      {
        _data[z] = new V*[h]; 
        for(unsigned y=0; y < h; ++y)
          _data[z][y] = &parent::data()[y*w + z*w*h];
      }
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::Signal3D(const Signal3D<V>& s)
    : parent(s)
    {
      unsigned w = s.domain()[0];
      unsigned h = s.domain()[1];
      unsigned d = s.domain()[2];
      _data = new V** [d];
      for(unsigned z=0; z < d; ++z)
      {
        _data[z] = new V*[h]; 
        for(unsigned y=0; y < h; ++y)
          _data[z][y] = &parent::data()[y*w + z*w*h];
      }
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::~Signal3D()
    {
      for(unsigned i=0; i < parent::_domain[2]; ++i)
        delete [] _data[i];
      delete [] _data;
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::     
    operator std::vector<V*>()
    {
      std::vector<V*> res;
      for(unsigned z=0; z < parent::_domain[2]; ++z)
        res.push_back(_data[z][0]);
      return res;
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::
    operator const std::vector<V*>() const
    {
      std::vector<V*> res;
      for(unsigned z=0; z < parent::_domain[2]; ++z)
        res.push_back(_data[z][0]);
      return res;
    }

    // =======================================================================
    template<typename V>
    Signal3D<V>::
    operator const std::vector<Signal2D<V> >() const
    {
      std::vector<Signal2D<V> > res;
      for(unsigned z=0; z < parent::_domain[2]; ++z)
        res.push_back(Signal2D<V>(spl::Domain<2>(parent::_domain[0],parent::_domain[1]),
                                  *this,
                                  _data[z][0]));
      return res;
    }


    // =======================================================================
    template<typename V>
    Signal3D<V>::
    operator const std::vector<V**>() const
    {
      std::vector<V**> res;
      for(unsigned i=0; i < parent::_domain[2]; ++i)
        res.push_back(_data[i]);
      return res;
    }
  }//!spl
