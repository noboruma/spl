# ifndef SPL_DIFFERENTIALS_HH
# define SPL_DIFFERENTIALS_HH

  namespace spl{
    namespace differentials 
    {
      //enum numeric { NUM_FORWARD, NUM_CENTERED } ;
      // Gradient (local operator -- one orientation)
      template <typename E>
      traits_value_type(E) gradient(const E &sig,
                                    const traits_point_type(E) &p,
                                    const unsigned i)
      {
        //switch (num)
        //{
        //  case NUM_FORWARD:
        //  {
            traits_point_type(E) q(p) ;
            q[i] = ( p[i] == sig.domain()[i]-1 ) ? p[i] : p[i]+1 ;
            return sig[q] - sig[p] ;
        //  }
        //  case NUM_CENTERED:
        //  {
        //    traits_point_type(E) q(p), r() ;
        //    q[i] = 
        //  }
        //}
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
        {
          std::cout << "life" << std::endl ;
          const E& tmp = gradient<E>(sig,i);
          std::cout << tmp(15,15) << std::endl ;
          for_each_pixel(grad(i),x,y)
          grad(i)(x,y) = 0;
        }
        return grad;
      }

      // Divergence (local operator -- one orientation)
      template <typename E>
      traits_value_type(E) divergence(const E &sig,
                                      const traits_point_type(E) &p,
                                      unsigned i)
      {
        traits_point_type(E) q(p) ;
        q[i] = ( p[i] == 0 ) ? p[i] : p[i]-1 ;
        return sig[p] - sig[q] ;
      }

      // Divergence (local operator -- all orientations)
      template <typename E>
      traits_value_type(E) divergence(const Signal1D<E> &sig,
                                      const traits_point_type(E) &p)
      {
        traits_value_type(E) div = 0 ;
        for_each_lin(sig,i)
          div += divergence(sig(i),p,i) ;
        return div ;
      }

      // Divergence (global operator -- all orientations)
      template <typename E>
      E divergence(const Signal1D<E> &sig)
      {
        E out(sig(0).domain()) ;
        traits_iterator_type(E) it(out.domain()) ;
        for_each_element(it)
          out[it] = divergence(sig,it) ;
        return out ;
      }

      // Laplacian
      template <typename E>
      E laplacian(const E &sig)
      {
        return divergence(gradient(sig)) ;
      }

    } //!differentials
  }//!spl

# endif
