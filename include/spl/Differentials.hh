# ifndef SPL_DIFFERENTIALS_HH
# define SPL_DIFFERENTIALS_HH

  namespace spl{
    namespace differentials 
    {
      // Gradient (local operator -- one orientation)
      template <typename E>
      traits_value_type(E) gradient(const E &sig,
                                    const traits_point_type(E) &p,
                                    unsigned i)
      {
        traits_point_type(E) q ;
        for (unsigned j = 0 ; j < traits_domain_dim(E) ; j++)
        {
          if (j==i)
            q[i] = ( p[i] == sig.domain()[i]-1 ) ? p[i] : p[i]+1 ;
          else
            q[j] = p[j] ;
        }
        return sig[q] - sig[p] ;
      }

      // Gradient (local operator -- all orientations)
      template <typename E>
      Signal1D<traits_value_type(E)> gradient(const E& sig,
                                              const traits_point_type(E) &p)
      {
        Signal1D<traits_value_type(E)> grad(traits_domain_dim(E)) ;
        for_each_lin(grad,i)
          grad(i) = gradient(sig,p,i) ;
        return grad ;
      }

      // Gradient (global operator -- one orientation)
      template <typename E>
      E gradient(const E& sig, unsigned i)
      {
        E out(sig.domain()) ;
        traits_iterator_type(E) it(sig.domain()) ;
        for_each_element(it)
          out[it] = gradient(sig,it,i) ;
        return out ;
      }

      // Gradient (global operator -- all orientations)
      template <typename E>
      Signal1D<E> gradient(const E& sig)
      {
        Signal1D<E> grad(traits_domain_dim(E), sig.domain());
        for_each_lin(grad,i)
          grad(i) = gradient(sig,i);
        return grad;
      }

//      template <typename E>
//      NDSignal<Signal1D<traits_value_type(E)>> gradient(E sig)
//      {
//        NDSignal<Signal1D<traits_value_type(E)>> grad(sig.domain()) ;
//        traits_iterator_type(NDSignal<traits_value_type(E)>) it (sig.domain()) ;
//        for_each_element(it)
//          grad[it] = spl::differentials::gradient(sig,it) ;
//        return grad ;
//      }

    } //!differentials
  }//!spl

# endif
