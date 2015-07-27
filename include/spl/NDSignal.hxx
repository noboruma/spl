  namespace spl
  {
    // =====================================================================
    template<typename E>
    template<unsigned dim>
    NDSignal<E>::NDSignal(const Domain<dim> dom)
    : _domain(dom)
    , _contiguous_data(nullptr, std::default_delete<traits_value_type(E)[]>())
    {
      for(unsigned i=0; i < traits_domain_dim(E); ++i)
        if(_domain[i] == 0)
          throw_logic("Signal data l-formed");
      _contiguous_data.reset(new traits_value_type(E)[dom.prod()],
                             std::default_delete<traits_value_type(E)[]>());
    }


  }//!spl
