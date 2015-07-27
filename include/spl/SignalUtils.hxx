  namespace spl{

    // =====================================================================
    template<typename S, template <typename> class Compare >
    void Extremum<S,Compare>::operator()()
    {
      const Compare<traits_value_type(S)> cmp 
      = Compare<traits_value_type(S)>();

      traits_iterator_type(S) it(_in.domain());

      _extremum = _in[it];
      _coord = it;

      for_each_element(it)
      {
        if(cmp(_in[it],_extremum))
        {
          _extremum = _in[it];
          _coord = it;
        }
      }
    }

    // =====================================================================
    template<typename S, template <typename> class Compare >
    void Extremum<S,Compare>::operator()(const mute(S,bool)& mask)
    {
      const Compare<traits_value_type(S)> cmp 
      = Compare<traits_value_type(S)>();

      traits_iterator_type(S) it(_in.domain());

      while(!mask[it])
        ++it;
      _extremum = _in[it];
      _coord = it;

      for_each_element(it)
      {
        if(!mask[it])
          continue;
        if(cmp(_in[it],_extremum))
        {
          _extremum = _in[it];
          _coord = it;
        }
      }
    }
    
    // =====================================================================
    template<typename V>
    void SubPixelShift<Signal2D<V> >::operator()(int x_shift_i,
                                                 int y_shift_i,
                                                 int x_shift_r,
                                                 int y_shift_r)
      {
        while(x_shift_r < 0)
        {
          --x_shift_i;
          x_shift_r += precision;
        }

        while(x_shift_r > precision)
        {
          ++x_shift_i;
          x_shift_r -= precision;
        }

        while(y_shift_r < 0)
        {
          --y_shift_i;
          y_shift_r += precision;
        }
        while(y_shift_r > precision)
        {
          ++y_shift_i;
          y_shift_r -= precision;
        }

        // Integer part
        for_each_pixel(_sig, x, y)
        {
          traits_point_type(Signal2D<V>) p(x,y);
          int x_shift = x+ x_shift_i,
              y_shift = y+ y_shift_i;
          if(x_shift < (int)_sig.domain()[0] 
             && y_shift < (int)_sig.domain()[1]
             && x_shift >= 0 
             && y_shift >= 0)
            _res(x,y) = _sig(x_shift, y_shift);
          else 
            _res(x,y) = _sig(x,y);
        }
        /*
         * Barycenter calcul
         *                Precision
         *          x_s
         *     -----------
         *     | wtl | wtr|
         *  y_s|----------|
         *     | wbl | wbr|
         */
        if(x_shift_r == 0 && y_shift_r == 0)
          return;

        long wbr=0, wtr=0, wbl=0, wtl=0,
             sqr = precision;
        if(x_shift_r != 0 && y_shift_r !=0)
        {
          wbr = x_shift_r * y_shift_r;
          wtl = (precision - x_shift_r) * (precision - y_shift_r);

          wbl = x_shift_r ;
          wbl *= (precision - y_shift_r);

          wtr = (precision - x_shift_r);
          wtr *= y_shift_r ;

          sqr *= precision;
        }
        else if (x_shift_r == 0)
        {
          wbr = 0;
          wtl = (precision - y_shift_r) ;
          wtr = y_shift_r;
          wbl =0;
        } 
        else
        {
          wbr = 0;
          wtl = (precision - x_shift_r) ;
          wtr = 0;
          wbl = x_shift_r;
        }

        for_each_pixel(_res, x, y)
        {
          int x_shift = x+1,
              y_shift = y+1;
          if(x_shift < (int)_res.domain()[0] 
             && y_shift < (int)_res.domain()[1]
             && y_shift >= 0)
          _res(x,y) = (long)((long)_res(x,y) * wtl +
                             (long)_res(x_shift, y) * wbl +
                             (long)_res(x, y_shift) * wtr +
                             (long)_res(x_shift, y_shift) * wbr)/sqr;
        }
      }
  namespace ct{

    // =========================================================================
    template<typename V, unsigned L>
    inline V BilinearInterpolation<V,L>::operator()(unsigned x, unsigned y)
    {
      assert(std::is_signed_cast_safe(x));
      assert(std::is_signed_cast_safe(y));

      double x1 = ::floor(x/(L+1));
      double y1 = ::floor(y/(L+1));
      double x2 = x1+1;
      double y2 = y1+1;

      double f11 = _sig(x1, y1);
      double f12 = _sig(x1, y2);
      double f21 = _sig(x2, y1);
      double f22 = _sig(x2, y2);

      x1 *= (L+1);
      y1 *= (L+1);
      x2 *= (L+1);
      y2 *= (L+1);

      double res = ((double)f11*(x2 - x) * (y2 - y)
                    + f21*(x - x1) * (y2 - y)  
                    + f12*(x2 - x) * (y - y1) 
                    + f22*(x - x1) * (y - y1) );
      res /= ((x2-x1)*(y2-y1));

      assert(std::is_cast_lossless<V>(res));

      return std::round(res);
    }
  } // !ct
  namespace rt{
    template<typename V>
    inline V BilinearInterpolation<V>::operator()(unsigned x, unsigned y)
    {
      assert(std::is_signed_cast_safe(x));
      assert(std::is_signed_cast_safe(y));

      double x1 = ::floor(x/(L+1));
      double y1 = ::floor(y/(L+1));
      double x2 = x1+1;
      double y2 = y1+1;

      double f11 = _sig(x1, y1);
      double f12 = _sig(x1, y2);
      double f21 = _sig(x2, y1);
      double f22 = _sig(x2, y2);

      x1 *= (L+1);
      y1 *= (L+1);
      x2 *= (L+1);
      y2 *= (L+1);

      double res = ((double)f11*(x2 - x) * (y2 - y)
                    + f21*(x - x1) * (y2 - y)  
                    + f12*(x2 - x) * (y - y1) 
                    + f22*(x - x1) * (y - y1) );
      res /= ((x2-x1)*(y2-y1));

      assert(std::is_cast_lossless<V>(res));

      return std::round(res);
    }
  }//!rt
}//!spl
