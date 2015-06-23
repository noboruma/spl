  namespace spl
  {
    // =======================================================================
    template<typename S>
    BoundMorpher<S>::BoundMorpher(const S &src,
                                  const traits_point_type(S) &from,
                                  const traits_point_type(S) &to)
    : parent(src)
      , _src(&src)
      , _from(from.exact())
      , _to(to.exact())
    {
      if(from[1] > to[1]
         || from[0] > to[0])
        throw_logic("ROI points not well ordonned");
      compute_rect_domain(from,to);
    }

    // =======================================================================
    template<typename S>
    void BoundMorpher<S>::compute_rect_domain(const traits_point_type(S) &from,
                                              const traits_point_type(S) &to)
    {
      for(unsigned i=0; i < traits_domain_dim(S); ++i)
      {
        const unsigned max = std::max(from[i], to[i]);
        const unsigned min = std::min(from[i], to[i]);
        parent::_domain[i] = max - min +1;
      }
    }

    // =======================================================================
    template<typename S>
    traits_concrete_type(S) BoundMorpher<S>::clone_impl() const
    {
      S res(parent::_domain);
      traits_iterator_type(S) it(parent::_domain);
      for_each_element(it)
        res[it] = (*this)[it];

      return res;
    }

    // =======================================================================
    template<typename S>
    void BoundMorpher<S>::operator=(const BoundMorpher<S> &m)
    {
      _src = m._src;
      parent::_domain = m._domain;
      parent::_contiguous_data = m._contiguous_data;
      _from = m._from;
      _to = m._to;
    }

  }//!spl
