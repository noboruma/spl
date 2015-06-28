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

    // =====================================================================
    template<typename E>
    template<unsigned dim, typename ... Args>
    NDSignal<E>::NDSignal(const Domain<dim> dom, Args ... args)
    : _domain(dom)
    , _contiguous_data(nullptr, std::default_delete<traits_value_type(E)[]>())
    {
      for(unsigned i=0; i < traits_domain_dim(E); ++i)
        if(_domain[i] == 0)
          throw_logic("Signal data l-formed");
      std::allocator<traits_value_type(E)> a;
      auto p = a.allocate(dom.prod());
      a.construct(p, args...); // Make this more feneric (ie : args[0] and recursive call...)
      _contiguous_data.reset(p, std::default_delete<traits_value_type(E)>());// TODO: understand why it does not require [] delete (Valgrind might be fooled)
    }

  }//!spl
