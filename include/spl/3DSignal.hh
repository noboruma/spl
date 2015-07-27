# ifndef SPL_3DSIGNAL_HH
# define SPL_3DSIGNAL_HH

# include "NDSignal.hh"
# include "Point.hh"
#include "2DSignal.hh"
#include <iostream>

namespace spl{
  template<typename V>
    struct Signal3D;

  namespace global{
    template<>
      template<typename V>
      struct traits<Signal3D<V> >
      {
        typedef V value_type;
        typedef Point3D point_type;
        typedef Domain<3> domain_type;
        typedef Signal2D<V>  sub_type;
        typedef Signal3D<V>  concrete_type;
        //constexpr auto axis_dims() {return std::make_index_sequence<3>();}
      };
    template<typename oldType, typename newVal>
      struct mutate<Signal3D<oldType>,oldType,newVal>
      {
        typedef Signal3D<newVal> res;
      };

  }//!global

  template <typename V>
    struct Signal3D : public NDSignal<Signal3D<V> >
  {
    typedef NDSignal<Signal3D<V> > parent;

    Signal3D(unsigned w, unsigned h, unsigned d);
    Signal3D(Domain<3> dom);
    Signal3D(const Signal3D<V>& s);

    ~Signal3D();

    V& operator()(unsigned x, unsigned y, unsigned z) 
    {
      return at_impl(traits_point_type(Signal3D<V>)(x,y,z));
    }
    const V& operator()(unsigned x, unsigned y, unsigned z) const 
    { 
      return at_impl(traits_point_type(Signal3D<V>)(x,y,z));
    }

    private:
    inline V &at_impl(const traits_point_type(Signal3D<V>)& p)
    {
      return _data[p._z][p._y][p._x];
    }
    inline const V &at_impl(const traits_point_type(Signal3D<V>)& p) const
    {
      return _data[p._z][p._y][p._x];
    }

    public:
    void operator=(const Signal3D<V>& p)
    {
      parent::operator=(p);
      unsigned h= p.domain()[1];
      unsigned d= p.domain()[2];
      _data = new V** [d];
      for(unsigned z=0; z < d; ++z)
      {
        _data[z] = new V*[h]; 
        for(unsigned y=0; y < h; ++y)
          _data[z][y] = p._data[z][y];
      }
    }
    const unsigned width() const {return parent::_domain[0];}
    const unsigned height() const {return parent::_domain[1];}
    const unsigned depth() const {return parent::_domain[2];}

    operator std::vector<V*>();

    operator const std::vector<V*>() const;

    operator const std::vector<Signal2D<V> >() const;
    //operator std::vector<Signal2D<V> >();

    operator const std::vector<V**>() const;

    private:
    inline Signal3D clone_impl() const
    {
      Signal3D ret(this->domain());
      for_each_voxel_par(ret, x, y, z)
        ret(x, y, z) = (*this)(x, y, z);
      return ret;
    }

    const Signal3D<double> div_comp_wise_impl(const Signal3D<V> &b) const
    {
      Signal3D<double> res(this->domain());
      // TODO: Add cast secure
      for_each_voxel_par((*this),x,y,z)
        res(x,y,z) = (double)(*this)(x,y,z) / (double)b(x,y,z);
      return res;
    }

    Signal3D();

    private:
    V ***_data;

    template <typename T>
    friend struct NDSignal;
  };
}//!spl

# include "3DSignal.hxx"

# endif
