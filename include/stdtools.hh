#ifndef STDC11_HH
#define STDC11_HH

#include <string>
#include <sstream>
#include <math.h>
#include <cassert>
#include <memory>
#include <iomanip>
#include <type_traits>
#include <limits>
#include <cmath>

/* **
 * This file is juste a set of convenient standard methods from std11 such
 * as smart pointer and other stuff.
 */

#define sign_form(T) typename std::make_signed<T>::type
#define sign_form_(T) std::make_signed<T>::type

namespace std
{
  template<bool exclude=true, typename T>
  bool approx_equal(T a, T b, T epsilon=std::numeric_limits<T>::epsilon())
  {
    if(a > b) { a ^= b; b ^= a; a ^= b; }

    if(exclude)
      return   (a > (b- epsilon) &&  a < (b + epsilon));
    else
      return   (a >= (b- epsilon) &&  a <= (b + epsilon));
  }

  template< typename T >
  inline T convert(const std::string& str)
  {
    std::istringstream iss(str);
    T obj;

    iss >> std::ws >> obj >> std::ws;

    if(!iss.eof())
      throw "dammit!";

    return obj; 
  }
  const double PI = 3.14159265;

  template<typename T>
  bool is_signed_cast_safe(T a)
  {
    static_assert(std::numeric_limits<T>::is_modulo, "type must be 2 complement");
    static_assert(!std::numeric_limits<T>::is_signed, "type must be unsigned");

    typedef typename std::make_signed<T>::type signed_T;
    typedef typename std::make_unsigned<T>::type unsigned_T;

    return a < (unsigned_T) std::numeric_limits<signed_T>::max() ;
    //&& a < (unsigned)std::numeric_limits<typename std::make_signed<T>::type>::max();
  }

  template<typename C, typename T>
  bool is_cast_lossless(T a)
  {
    if(std::numeric_limits<C>::digits >= std::numeric_limits<T>::digits)
      return true;

    static_assert(std::numeric_limits<C>::is_modulo, "type must be 2 complement");

    return a <= static_cast<T>(std::numeric_limits<C>::max()) && 
           a >= static_cast<T>(std::numeric_limits<C>::min());
  }

  template <bool B, typename T1, typename T2>
  struct IF;
  template <typename T1, typename T2>
  struct IF<false,T1,T2>
  {
    typedef T2 res;
  };
  template <typename T1, typename T2>
  struct IF<true,T1,T2>
  {
    typedef T1 res;
  };

  template <typename T>
  std::string to_string (T num)
  {
    std::ostringstream ss;
    ss << std::setprecision(3) << num;
    return ss.str();
  }

  template<typename T>
  T round(T v)
  {
    return ::round(v);
  }

  template<typename T,typename ContainerT>
  void tokenize(const std::string &str,
                ContainerT &tokens,
                const std::string &delimiters = " ",
                bool trimEmpty = false)
  {
    std::string::size_type pos, lastPos = 0;
    while(true)
    {
      pos = str.find_first_of(delimiters, lastPos);
      if(pos == std::string::npos)
      {
        if(lastPos != str.length())
          tokens.push_back(std::string(str.data()+lastPos,
                                       (sizeof(T))*(str.length()-lastPos) ));
          break;
      }
      else
        if(pos != lastPos || !trimEmpty)
          tokens.push_back(std::string(str.data()+lastPos,
                                       (sizeof(T))*(pos-lastPos) ));
      lastPos = pos + 1;
    }
  }

}

#endif

