# ifndef SPL_CL_2D_SIGNAL_HH
# define SPL_CL_2D_SIGNAL_HH

# include "../NDSignal.hh"
# include "../Point.hh"
# include <vector>
#include "CLSignal.hh"

namespace spl
{
  namespace cl 
  {
    template<typename V>
    struct Signal2D;
  } //!cl
  namespace global
  {
    template<>
      template<typename V>
      struct traits<cl::Signal2D<V> >
      {
        typedef V value_type;
        typedef Point2D point_type;
        typedef Domain<2> domain_type;
        //typedef Signal1D<V> sub_type;
        typedef cl::Signal2D<V> concrete_type;
      };

    template<typename oldType, typename newVal>
      struct mute<cl::Signal2D<oldType>,oldType,newVal>
      {
        typedef cl::Signal2D<newVal> res;
      };
  }//!global

  namespace cl 
  {
    
    template <typename V>
    struct Signal2D : public NDSignal<Signal2D<V> >, public CLSignal<V>
    {
      typedef NDSignal<Signal2D<V> > parent;

      Signal2D(unsigned w, unsigned h, const ::cl::Device &dev);
      //Signal2D(Domain<2> dom);
      //Signal2D(const Signal2D<V>& s);
      //Signal2D(traits_domain_type(Signal2D<V>) dom,
      //       const spl::NDSignal<Signal3D<V> > &p,
      //       V * data);

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

      //void operator=(const Signal2D<V>& p)
      //{
      //  unsigned h= p.domain()[1];
      //  _data = new V* [h];
      //  for(unsigned y=0; y < h; ++y)
      //    _data[y] = &parent::data()[y*h];
      //  parent::operator=(p);
      //}


      private:
      V& at_impl(const traits_point_type(Signal2D<V>)& p)
      {
        return _data[p._y][p._x];
      }

      const V& at_impl(const traits_point_type(Signal2D<V>)& p) const
      {
        return _data[p._y][p._x];
      }

      //Signal2D clone_impl() const
      //{
      //  Signal2D ret(this->domain());
      //  for_each_pixels_par(ret, x, y)
      //    ret(x,y) = (*this)(x,y);
      //  return ret;
      //}

      public:
      const unsigned width() const {return parent::_domain[0];}
      const unsigned height() const {return parent::_domain[1];}

      //operator const V**() const
      //{
      //  return const_cast<const V**>(_data);
      //}

      //operator V*()
      //{
      //  return _data[0];
      //}

      //operator const V*() const
      //{
      //  return _data[0];
      //}

      private:
      //const Signal2D<double> div_comp_wise_impl(const Signal2D<V> &b) const
      //{
      //  Signal2D<double> res(this->domain());
      //  for_each_pixels_par((*this),x,y)
      //  //TODO: Add cast check
      //    res(x,y) = (double)(*this)(x,y) / (double)b(x,y);
      //  return res;
      //}

      private:
      V **_data;

      template <typename T>
      friend struct NDSignal;
    };
  } //!cl
}//!spl

#include "2DSignal.hxx"

# endif

