# ifndef SPL_MEAN_HH
# define SPL_MEAN_HH

#include "stdtools.hh"
#include "Exceptions.hh"
#include "BoundMorpher.hh"

  namespace spl{
    namespace algo{

      /* **
       * Mean calculate the mean value of a Signal
       * The mean might be calculate by two different ways:
       * - Excluding an axis
       * - Along side an axis
       * When calculating the mean, we can say we want the mean value along the N'th axis
       * Or the mean of the other axis on the Nth one.
       */
      template<typename IS,
      typename returnType=double,
      unsigned axis=traits_domain_dim(IS)-1,
      bool excluse=true>
      struct Mean
      {};

      template<typename IS, typename RT, unsigned axis>
      struct Mean<IS, RT, axis, true>
      {
        Mean(const IS& signal);
        void operator()();
        typedef mute(traits_sub_type(IS), RT) sub_type;
        const sub_type &res() {return *_res;}

        private:
        const IS& _src;
        std::unique_ptr<sub_type> _res;

      };

      template<typename IS, typename RT, unsigned axis>
      struct Mean<IS, RT, axis, false>
      {
        Mean(const IS& signal);
        void operator()();
        void operator()(const spl::Signal2D<bool>& mask);

        typedef Signal1D<RT> sub_type;
        const sub_type &res() {return _res;}

        private:
        const IS& _src;
        sub_type _res;
      };
    }//!algo


    //template<typename S>
    //  traits_value_type(S) 
    //  full2DMean(const S& in)
    //  {
    //    algo::Mean<S> m(in);
    //    m();
    //    algo::Mean<traits_sub_type(S)> m2(m.res());
    //    m2();
    //    return m2.res();
    //  }

  }//!spl

#include "Mean.hxx"

# endif
