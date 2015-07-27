# ifndef SPL_ANY_HH
# define SPL_ANY_HH

namespace spl{
  namespace global{
    template<typename E>
    struct Any{
      E& exact()
      {
        return static_cast<E&>(*this);
      }
      const E& exact() const
      {
        return static_cast<const E&>(*this);
      }
    };
  }// !global
}//!spl

#define __spl_impl(fct) this->exact().fct##_impl

#define for_un(i,beg,end)\
  for(unsigned i=beg; i < end; ++i)

# endif
