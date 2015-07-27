#ifndef SPL_CT_METATOOLS_HH
#define SPL_CT_METATOOLS_HH
namespace spl 
{
  namespace ct // compile-time
  {

    auto list = [](auto ...xs) 
    {
      return [=](auto access) { return access(xs...); };
    };

    auto length = [](auto xs) 
    {
      return xs([](auto ...z) { return sizeof...(z); });
    };

    namespace internal 
    {
      template <size_t N, typename... T>
      struct get;

      template <typename T0, typename... T>
      struct get<0, T0, T...> 
      {
        T0 value;
        get(T0 t0, T... t)
        : value(t0) {}
      };

      template <size_t N, typename T0, typename... T>
      struct get<N, T0, T...> : public get<N-1, T...>
      {
        get(T0 t0, T... t)
        : get<N-1, T...> (t...) {}
      };

      template <class F, class tuple, std::size_t... i>
      constexpr decltype(auto) unpack_tuple(F&& f, tuple&& t, std::index_sequence<i...>)
      {
        return std::forward<F>(f)(std::get<i>(std::forward<tuple>(t))...);
      }
    } //!internal

    template <class F, class Tuple>
    constexpr decltype(auto) unpack_tuple(F&& f, Tuple&& t)
    {
      return internal::unpack_tuple(std::forward<F>(f), std::forward<Tuple>(t),
                             std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>{});
    }

    template <size_t N>
    auto get(auto xs)
    {
      return xs([] (auto... list) { return internal::get<N, decltype(list)...>(list...).value;});
    }

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


    template<typename T, template<typename> class M>
    struct mutate;  

    //template <typename T, typename Y>
    //struct transform
    //{
    //  using type = Y;
    //};

    template<typename ...T, template<typename U> class M>
    struct mutate<std::tuple<T...>, M>  
    {
      using type = std::tuple<typename M<T>::type...>;
    };
  }//!ct
} //!spl
#endif
