# ifndef SPL_2D_SIGNAL_HH
# define SPL_2D_SIGNAL_HH

# include "NDSignal.hh"
# include "Point.hh"
# include "1DSignal.hh"
# include <vector>

namespace spl{
  template<typename V>
    struct Signal2D;
  template<typename V>
    struct Signal3D;
  namespace global{
    template<>
      template<typename V>
      struct traits<Signal2D<V> >
      {
        typedef V value_type;
        typedef Point2D point_type;
        typedef Domain<2> domain_type;
        typedef Signal1D<V> sub_type;
      };

    template<typename oldType, typename newVal>
      struct mute<Signal2D<oldType>,oldType,newVal>
      {
        typedef Signal2D<newVal> res;
      };
  }//!global

  template <typename V>
    struct Signal2D : public NDSignal<Signal2D<V> >
  {
    typedef NDSignal<Signal2D<V> > parent;

    Signal2D(unsigned w, unsigned h);
    Signal2D(Domain<2> dom);
    Signal2D(const Signal2D<V>& s);
    Signal2D(traits_domain_type(Signal2D<V>) dom,
             const spl::NDSignal<Signal3D<V> > &p,
             V * data);

    public:

    ~Signal2D();

    V& operator()(unsigned x, unsigned y) 
    { 
      return at_impl(traits_point_type(Signal2D<V>)(x,y));
    }

    const V& operator()(unsigned x, unsigned y) const 
    {
      return at_impl(traits_point_type(Signal2D<V>)(x,y));
    }

    V& at_impl(const traits_point_type(Signal2D<V>)& p)
    {
      return _data[p._y][p._x];
    }

    const V& at_impl(const traits_point_type(Signal2D<V>)& p) const
    {
      return _data[p._y][p._x];
    }

    const unsigned width() const {return parent::_domain[0];}
    const unsigned height() const {return parent::_domain[1];}

    operator const V**() const
    {
      return const_cast<const V**>(_data);
    }

    operator V*()
    {
      return _data[0];
    }

    operator const V*() const
    {
      return _data[0];
    }

    private:
    V **_data;
  };
}

#include "2DSignal.hxx"

# endif

