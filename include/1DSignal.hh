# ifndef SPL_1D_SIGNAL_HH
# define SPL_1D_SIGNAL_HH

# include "NDSignal.hh"
# include "Point.hh"
# include <vector>

namespace spl{
  template<typename V>
    struct Signal1D;

  namespace global{
    template<>
      template<typename V>
      struct traits<Signal1D<V> >
      {
        typedef V value_type;
        typedef Point1D point_type;
        typedef Domain<1> domain_type;
        typedef V sub_type;
        typedef Signal1D<V> concrete_type;
      };

    template<typename oldType, typename newVal>
      struct mute<Signal1D<oldType>,oldType,newVal>
      {
        typedef Signal1D<newVal> res;
      };
  }//!global

  /* **
   * Signal1D is a signal defined by only one dimension.
   * This structure is more like a 1D array.
   */
  template <typename V>
    struct Signal1D : public NDSignal<Signal1D<V> >
  {
    typedef NDSignal<Signal1D<V> > parent;

    Signal1D(unsigned w);
    Signal1D(Domain<1> dom);
    Signal1D(const spl::Signal1D<V> &sig);


    V& operator()(unsigned x) 
    { 
      return at_impl(traits_point_type(Signal1D<V>)(x));
    }

    const V& operator()(unsigned x) const 
    {
      return at_impl(traits_point_type(Signal1D<V>)(x));
    }

    V& at_impl(const traits_point_type(Signal1D<V>)& p)
    {
      return _data[p._x];
    }

    const V& at_impl(const traits_point_type(Signal1D<V>)& p) const
    {
      return _data[p._x];
    }

    const unsigned length() const 
    {
      return parent::_domain[0];
    }

    explicit operator V* ()
    {
      return _data;
    }

    explicit operator const V* () const
    {
      return _data;
    }

    Signal1D clone_impl() const
    {
      Signal1D ret(this->domain());
      for_each_lin(ret, x)
        ret(x) = (*this)(x);
      return ret;
    }


    private:
    V *_data;
  };
}//!spl

#include "1DSignal.hxx"

# endif

