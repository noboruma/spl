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

      for_each_elements(it)
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

      for_each_elements(it)
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
        const unsigned precision = 10;
        if(x_shift_r < 0)
        {
          --x_shift_i;
          x_shift_r += precision;
        }
        if(y_shift_r < 0)
        {
          --y_shift_i;
          y_shift_r += precision;
        }

        // Integer part
        for_each_pixels(_sig, x, y)
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

        for_each_pixels(_res, x, y)
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
  }//!spl
