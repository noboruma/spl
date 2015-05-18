#ifndef SPL_CT_METATOOLS_HH
#define SPL_CT_METATOOLS_HH
namespace spl 
{

  namespace ct // compile-time
  {
    template<typename F, unsigned I>
    void apply_for(const F& f)
    {}

    template<typename F, unsigned I=0, typename T, typename ...Args>
    void apply_for(const F& f, T&& e, Args&&... args)
    {
      f(I, e);
      apply_for<F, I+1>(f, args...);
    }
  }//!ct

} //!spl
#endif
