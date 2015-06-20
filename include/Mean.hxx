  namespace spl{
    namespace algo{

      // =======================================================================
      template<typename IS, typename returnType, unsigned axis>
      Mean<IS, returnType, axis, true>::Mean(const IS& signal)
      : _src(signal)
        , _res(nullptr)
      {
        traits_domain_type(sub_type) dom;

        unsigned cpt =0;
        for(unsigned i=0; i < traits_domain_dim(IS); ++i)
          if(i != axis)
            dom[cpt++] = _src.domain()[i];

        _res.reset(new sub_type(dom));
      }

      // =======================================================================
      template<typename IS, typename returnType, unsigned axis>
      void Mean<IS, returnType, axis, true>::operator()()
      {
        traits_iterator_type(sub_type) it((*_res).domain());
        if(_src.domain()[axis] == 0)
          throw_logic("Mean on null axis");

        for_each_elements(it)
        {
          (*_res)[it] = 0;

          traits_point_type(IS) p;

          unsigned cpt =0;
          for(unsigned i=0; i < traits_domain_dim(IS); ++i)
            if(i != axis)
              p[i] = it[cpt++];

          double acc = 0;
          for(unsigned dim=0; dim < _src.domain()[axis]; ++dim)
          {
            p[axis] = dim;
            acc += (double)_src[p] / (double)_src.domain()[axis];
          }
          (*_res)[it] = acc;
        }
      }

      // =======================================================================
      template<typename IS, typename returnType, unsigned axis>
      Mean<IS, returnType, axis,false>::Mean(const IS& signal)
      : _src(signal)
        , _res(sub_type(signal.domain()[axis]))
      {}
      // =======================================================================
      template<typename IS, typename returnType, unsigned axis>
      void Mean<IS, returnType, axis, false>::operator()()
      {
        traits_point_type(IS) from,to;
        for(unsigned i=0; i < traits_domain_dim(IS); ++i)
          if(i != axis)
            to[i] = _src.domain()[i] -1;
        for(unsigned dim = 0; dim < _src.domain()[axis]; ++dim)
        {
          const BoundMorpher<IS> area(_src,from,to);
          traits_iterator_type(BoundMorpher<IS>) it(area.domain());

          _res[dim] = 0;
          long double acc = 0;
          //long int cpt=0;
          for_each_elements(it)
          {
            acc = acc + (double)area[it] / (double)area.domain().prod();
          }
          //if(cpt != 0) // TODO : verify limits on sum / 
            //_res[dim] = ((long double)acc / (long double)cpt);
          _res[dim] = acc;
          ++from[axis];
          ++to[axis];
        }
      }

      // =======================================================================
      template<typename IS, typename returnType, unsigned axis>
      void Mean<IS, returnType, axis, false>::operator()(const spl::Signal2D<bool>& mask)
      {
        traits_point_type(IS) from,to;
        // determine which coord to use on the 2D mask from the 3D iterator
        unsigned coord_mask_from_iterator[2] = {0,0};
        unsigned tmp_cpt = 0;

        for(unsigned i=0; i < traits_domain_dim(IS); ++i)
          if(i != axis)
          {
            to[i] = _src.domain()[i] -1;
            coord_mask_from_iterator[tmp_cpt++] = i;
          }
        for(unsigned dim = 0; dim < _src.domain()[axis]; ++dim)
        {
          const BoundMorpher<IS> area(_src,from,to);
          traits_iterator_type(BoundMorpher<IS>) it(area.domain());

          _res[dim] = 0;
          long int acc = 0;
          long int cpt=0;
          for_each_elements(it)
          {
            if(!mask[spl::Point2D(it[coord_mask_from_iterator[0]],
                                  it[coord_mask_from_iterator[1]])])
              continue;
#ifndef NDEBUG
            long int tmp = acc;
#endif
            acc += area[it];
#ifndef NDEBUG
            assert(acc > tmp);
#endif
            ++cpt;
          }

          if(cpt != 0) // TODO : verify limits on sum / 
            _res[dim] = ((long double)acc / (long double)cpt);
          ++from[axis];
          ++to[axis];
        }
      }
    }//!algo
  }//!spl

