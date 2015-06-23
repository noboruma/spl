# ifndef SPL_SIGNAL_UTILS_HH
# define SPL_SIGNAL_UTILS_HH

#include "NDSignal.hh"
#include <functional>
#include <list>

// Returns coordonate where max have been found in S input

  namespace spl{

    /* **
     * Extermum retrieve an extremum according to the Compare functor onto a NDSignal.
     */
    template<typename S, template <typename> class Compare >
    struct Extremum{
      Extremum(const S& in)
      : _in(in)
      {}

      void operator()();
      void operator()(const mute(S,bool)& mask);


      inline const traits_point_type(S) &res_point() {return _coord;}
      inline const traits_value_type(S) &res() {return _extremum;}

      private:
      const S &_in;
      traits_value_type(S) _extremum;
      traits_point_type(S) _coord;
    };

    /* **
     * PolygonMask calculate a 2D binary mask according to ordered polygon points.
     */
    struct PolygonMask
    {
      typedef std::list<spl::Point2D> polygon_type;

      PolygonMask(const Domain<2>& dom,
                  const polygon_type &l)
      : _res(dom)
        , _list(l)
      {}

      private:
      bool insidePolygon(const Point2D &p)
      {
        std::vector<spl::Point2D> polygon_points;
        polygon_points.reserve(_list.size());
        polygon_type::const_iterator it = _list.begin();
        for (it =  _list.begin();
             it != _list.end();
             ++it)
          polygon_points.push_back(*it);

        //unsigned int counter = 0;
        bool odd_counter = false;

        unsigned N = polygon_points.size();
        unsigned j=N-1;
        double x = p[0],y = p[1];

        for (unsigned i=0; i<polygon_points.size(); i++) 
        {
          if ((polygon_points[i][1]<y && polygon_points[j][1]>=y)
              || ( polygon_points[j][1]<y && polygon_points[i][1]>=y)) 
          {
            if ((double)polygon_points[i][0]
                +(((double)y-(double)polygon_points[i][1])
                  /((double)polygon_points[j][1]-(double)polygon_points[i][1]))
                *((double)polygon_points[j][0]-(double)polygon_points[i][0]) > (double)x) 
              odd_counter=!odd_counter; 
          }
          j=i; 
        }
        return odd_counter;
      }
      public:

      void operator()()
      {
        traits_iterator_type_(Signal2D<bool>) it(_res.domain());
        for_each_element(it)
        _res[it] = insidePolygon(it);
      }

      const Signal2D<bool> &res() const {return _res;} 

      private:
      spl::Signal2D<bool> _res;
      const polygon_type &_list;
    };

    template<typename V>
    struct SubPixelShift{};
    
    template<typename V>
    struct SubPixelShift<Signal2D<V> >
    {

      SubPixelShift(const Signal2D<V>& sig)
      : _sig(sig)
      , _res(sig.domain())
      {}

      void operator()(int x_shift_i, int y_shift_i, int x_shift_r, int y_shift_r);

      const Signal2D<V> &res(){
        return _res;
      }

      static const signed precision = 10;
      private:
      const Signal2D<V> &_sig;
      Signal2D<V> _res;
    };



    namespace ct
    {
      template<typename V, unsigned L>
      struct BilinearInterpolation
      {
        const Signal2D<V>& _sig;
        BilinearInterpolation(const Signal2D<V>& sig)
        : _sig(sig)
        {}
        inline V operator()(unsigned x, unsigned y);
      };
    }//!ct
    namespace rt
    {
      template<typename V>
      struct BilinearInterpolation
      {
        const Signal2D<V>& _sig;
        const unsigned L;
        BilinearInterpolation(const Signal2D<V>& sig, unsigned L_)
        : _sig(sig)
        , L(L_)
        {}
        inline V operator()(unsigned x, unsigned y);
      };
    }//!rt

    template<typename V, unsigned L=0>
    struct BilinearInterpolation : public ct::BilinearInterpolation<V,L>
    {
      typedef ct::BilinearInterpolation<V,L> parent;
      //using  ct::BilinearInterpolation<V,L>::ct::BilinearInterpolation<V,L>; // C++11 gcc v4.8 only...
      BilinearInterpolation(const Signal2D<V>&sig)
      : parent(sig)
      {}
    };

    template<typename V>
    struct BilinearInterpolation<V,0> : public rt::BilinearInterpolation<V>
    {
      typedef rt::BilinearInterpolation<V> parent;
      //using parent::parent; // Gcc 4.8 only
      BilinearInterpolation(const Signal2D<V>&sig, unsigned L_)
      : parent(sig, L_)
      {}
    };

    template<typename PointType, typename DomainType>
    PointType unscale_point(PointType pos,
                            DomainType scaledDomain,
                            DomainType unScaledDomain,
                            PointType org = PointType(),
                            DomainType scaledBorderOffset = DomainType(),
                            PointType zoomPoint = PointType())
    {
#ifdef DEBUG
      for(unsigned i=0; i < DomainType::dim; ++i)
      {
        assert(scaledDomain[i] >= 0);
        assert(unScaledDomain[i] >= 0);
        assert(scaledBorderOffset[i] >= 0);
      }
#endif
      double ratios[DomainType::dim];
      for(unsigned i=0; i < DomainType::dim; ++i)
        ratios[i] = (double)unScaledDomain[i]/(double)(scaledDomain[i] - scaledBorderOffset[i]*2);

      PointType offseted_p = pos - org;

      for(unsigned i=0; i < DomainType::dim; ++i)
      {
        offseted_p[i] = std::round((double)offseted_p[i]-scaledBorderOffset[i])*ratios[i];
        offseted_p[i] = std::min(offseted_p[i], unScaledDomain[i]);
        offseted_p[i] = std::max(offseted_p[i], 0);
      }

      return offseted_p + zoomPoint;
    }

  }//!spl

#include "SignalUtils.hxx"

# endif
