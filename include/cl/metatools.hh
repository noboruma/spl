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

    template<template<typename> class cmp, unsigned I>
    void if_apply_for(const auto &f1, const auto &f2)
    {}

    template<template<typename> class cmp, unsigned I=0, typename T, typename ...Args>
    void if_apply_for(const auto &then_, const auto &else_, T& e, Args&&... args)
    {
      if(cmp<T>::value)
        then_(I, e);
      else
        else_(I,e);
      if_apply_for<cmp, I+1>(then_, else_, args...);
    }
  }//!ct
} //!spl
#endif
