# ifndef SPL_3DSIGNAL_HH
# define SPL_3DSIGNAL_HH

# include "NDSignal.hh"
# include "Point.hh"
#include "2DSignal.hh"

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
      };
    template<typename oldType, typename newVal>
      struct mute<Signal3D<oldType>,oldType,newVal>
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

    ~Signal3D();

    V& operator()(unsigned x, unsigned y, unsigned z) 
    {
      return at_impl(traits_point_type(Signal3D<V>)(x,y,z));
    }
    const V& operator()(unsigned x, unsigned y, unsigned z) const 
    { 
      return at_impl(traits_point_type(Signal3D<V>)(x,y,z));
    }

    inline V &at_impl(const traits_point_type(Signal3D<V>)& p)
    {
      return _data[p._z][p._y][p._x];
    }
    inline const V &at_impl(const traits_point_type(Signal3D<V>)& p) const
    {
      return _data[p._z][p._y][p._x];
    }

    const unsigned width() const {return parent::_domain[0];}
    const unsigned height() const {return parent::_domain[1];}
    const unsigned depth() const {return parent::_domain[2];}

    operator std::vector<V*>();

    operator const std::vector<V*>() const;

    operator const std::vector<Signal2D<V> >() const;
    //operator std::vector<Signal2D<V> >();

    operator const std::vector<V**>() const;

    Signal3D clone_impl() const
    {
      Signal3D ret(this->domain());
      for_each_voxels_par(ret, x, y, z)
        ret(x, y, z) = (*this)(x, y, z);
      return ret;
    }

    const Signal3D<double> div_comp_wise_impl(const Signal3D<V> &b)
    {
      Signal3D<double> res(this->domain());
      for_each_voxels_par(*this,x,y,z)
        res(x,y,z) = (*this)(x,y,z) / b(x,y,z);
      return res;
    }

    private:
    V ***_data;
  };
}//!spl

# include "3DSignal.hxx"

# endif
