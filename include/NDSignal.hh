# ifndef SPL_ND_SIGNAL_HH
# define SPL_ND_SIGNAL_HH

# include "any.hh"
# include "Domain.hh"
//#include <iostream>
# include "stdtools.hh"
# include "Exceptions.hh"
# include <deque>

  namespace spl{

    template<typename S>
    struct BoundMorpher;

    namespace global{
      template<typename E>
      struct traits;
      template<typename E, typename oV, typename V>
      struct mute;
    }// !global

#define traits_value_type(E) typename spl::global::traits<E>::value_type
#define traits_point_type(E) typename spl::global::traits<E>::point_type
#define traits_domain_type(E) typename spl::global::traits<E>::domain_type
#define traits_sub_type(E) typename spl::global::traits<E>::sub_type
#define traits_concrete_type(E) typename spl::global::traits<E>::concrete_type
#define traits_iterator_type(E) typename E::iterator

#define traits_value_type_(E) spl::global::traits<E>::value_type
#define traits_point_type_(E) spl::global::traits<E>::point_type
#define traits_domain_type_(E) spl::global::traits<E>::domain_type
#define traits_sub_type_(E) spl::global::traits<E>::sub_type
#define traits_iterator_type_(E) E::iterator

#define traits_domain_dim(E) spl::global::traits<E>::domain_type::dim

#define traits_mute(TplType, newValueType) typename spl::global::mute<TplType, traits_value_type(TplType), newValueType>::res
#define mute(TplType, newValueType) typename spl::global::mute<TplType, traits_value_type(TplType), newValueType>::res
#define mute_(TplType, newValueType) spl::global::mute<TplType, traits_value_type_(TplType), newValueType>::res

#define for_each_lin(sig,i)\
    for(unsigned i=0; i < sig.length(); ++i)

#define for_each_pixels(im, i, j)\
    for(unsigned j=0; j < im.domain()[1]; ++j)\
    for(unsigned i=0; i < im.domain()[0]; ++i)

#define for_each_inner_pixels(im, i, j, b)\
    for(unsigned j=b; j < im.domain()[1]-b; ++j)\
    for(unsigned i=b; i < im.domain()[0]-b; ++i)

#define for_each_pixels_par(im, i, j)\
    _Pragma("omp parallel for collapse(2)")\
    for_each_pixels(im, i, j)

#define for_each_voxels(vol, i, j, k)\
for(unsigned k=0; k < vol.domain()[2]; ++k)\
  for(unsigned j=0; j < vol.domain()[1]; ++j)\
    for(unsigned i=0; i < vol.domain()[0]; ++i)

#define for_each_voxels_par(im, i, j, k)\
    _Pragma("omp parallel for collapse(3)")\
    for_each_voxels(im, i, j, k)

#define for_each_elements(it)\
    for(it.begin();it.end();++it)

    /* **
     * NDSignal is a structure to represent every type of N dimension Signal
     */
    template<typename E>
    struct NDSignal : public global::Any<E>
    {

      protected:

      template<unsigned dim>
      NDSignal(const Domain<dim> dom);

      // Down grade
      template<unsigned dim, typename V>
      NDSignal(const Domain<dim> dom, const NDSignal<V> &p)
      : _domain(dom)
      , _contiguous_data(nullptr)
      {
        _contiguous_data = p._contiguous_data;
      }

      /**
       * Weak copy
       */
      template<typename V>
      NDSignal(const NDSignal<V> &p)
      : _domain(p._domain)
      , _contiguous_data(nullptr)
      {
        _contiguous_data = p._contiguous_data;
      }

      void operator=(const NDSignal<E> &p)
      {
        _domain = p._domain;
        _contiguous_data = p._contiguous_data;
      }

      private:
      NDSignal();
      public:

      inline traits_value_type(E)& operator[](const traits_point_type(E) &p)
      {
        return __spl_impl(at)(p);
      }

      inline const traits_value_type(E)& operator[](const traits_point_type(E) &p) const
      {
        return __spl_impl(at)(p);
      }
      inline const traits_domain_type(E)& domain() {return _domain;}
      inline const traits_domain_type(E)& domain() const {return _domain;}

      inline traits_concrete_type(E) clone() const
      {
        return __spl_impl(clone)();
      }

      protected:

      traits_value_type(E) *data() 
      {
        return reinterpret_cast<traits_value_type(E)*>(_contiguous_data.get());
      }
      traits_value_type(E) *data() const
      {
        return reinterpret_cast<traits_value_type(E)*>(_contiguous_data.get());
      }

      inline const mute(traits_concrete_type(E),double) operator/ (const traits_concrete_type(E) &b)
      {
        return __spl_impl(div_comp_wise)(b);
      }

      protected:
      traits_domain_type(E) _domain;
      private:
      std::shared_ptr<char> _contiguous_data;

      template<typename S>
      friend class BoundMorpher;

      template <typename T>
      friend struct NDSignal;

      public:
      /* **
       * iterator to go through signals
       */
      struct iterator : public traits_point_type_(E)
      {

        iterator(const traits_domain_type(E)& dom)
        : traits_point_type_(E)()
          , _dom(dom)
          , stop(false) {};

        void operator++()
        {
          unsigned overflow = 1;
          do
          {
            for(unsigned dim=0; dim < traits_domain_dim(E) && overflow; ++dim)
              if((*this)[dim] + overflow >= _dom[dim])
              {
                (*this)[dim] = 0; 
                overflow = 1;
              }
              else
              {
                (*this)[dim] += 1;
                overflow = 0;
              }
            unsigned cpt= 0;
            for(unsigned dim=0; dim < traits_domain_dim(E) && !stop; ++dim)
              cpt += (*this)[dim];
            stop = cpt == 0;
          }while(overflow && !stop);
        }

        inline bool end()
        {
          return !stop;
        }

        void begin()
        {
          for(unsigned dim=0; dim < traits_domain_dim(E); ++dim)
            (*this)[dim] = 0;
        }

        private:
        const traits_domain_type(E) _dom;
        bool stop;
      };
    };
  }//!spl

# include "NDSignal.hxx"


# endif
