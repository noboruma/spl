# ifndef SPL_BOUND_MORPHER_HH
# define SPL_BOUND_MORPHER_HH

#include "Point.hh"
#include "NDSignal.hh"
#include "1DSignal.hh"
#include "2DSignal.hh"
#include "3DSignal.hh"
#include "Exceptions.hh"

  namespace spl{

    template<typename S>
    struct BoundMorpher;

    namespace global{
      template<>
      template<typename S>
      struct traits<BoundMorpher<S> >
      {
        typedef traits_value_type(S) value_type;
        typedef traits_point_type(S) point_type;
        typedef traits_domain_type(S) domain_type;
        typedef traits_sub_type(S) sub_type;
        typedef traits_concrete_type(S) concrete_type;
      };

      template<typename oldType, typename newVal>
      struct mutate<BoundMorpher<oldType>,oldType,newVal>
      {
        typedef typename mutate<oldType, traits_value_type(oldType), newVal>::res res;
      };
    }//!global

    template <typename S>
    struct BoundMorpher : public NDSignal<BoundMorpher<S> >
    {
      typedef NDSignal<BoundMorpher<S> > parent;

      // Segmentation points must be think as depalcement points. X means row and Y column, which is counterintuitive.
      BoundMorpher(const S &src,
                   const traits_point_type(S) &from,
                   const traits_point_type(S) &to);

      //inline traits_value_type(S) at_impl(traits_point_type(S)& p) const
      //{
      //  return (*_src)[_from+p];
      // }

      inline const traits_value_type(S)& at_impl(const traits_point_type(S)& p) const
      {
        return (*_src)[_from+p];
      }

      inline const traits_point_type(S)& fromBoundPoint() const {return _from;}
      inline const traits_point_type(S)& toBoundPoint()   const {return _to;}

      traits_concrete_type(S) clone_impl() const;

      void operator=(const BoundMorpher<S> &m);

      void translate(const traits_point_type(S) &translation)
      {
        for(unsigned i=0; i < traits_domain_dim(S); ++i)
          if( (_to[i] + translation[i] < _src->domain()[i])
             && ((signed)_from[i] + (signed)translation[i] >= 0))
          {
            _from[i] = _from[i] + translation[i];
            _to[i] = _to[i] + translation[i];
          }
      }


      const S& src() const { return *_src; }

      private:
      void compute_rect_domain(const traits_point_type(S) &from,
                               const traits_point_type(S) &to);
      const S *_src;
      traits_point_type(S) _from;
      traits_point_type(S) _to;
    };
  }//!spl

# include "BoundMorpher.hxx"

# endif
