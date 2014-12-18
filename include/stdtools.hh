#ifndef STDC11_HH
#define STDC11_HH

#include <string>
#include <sstream>
#include <math.h>
#include <cassert>
#include <memory>
#include <iomanip>

/* **
 * This file is juste a set of convenient standard methods from std11 such
 * as smart pointer and other stuff.
 */

namespace std{
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
        break;
      else
      {
        if(pos != lastPos || !trimEmpty)
          tokens.push_back(T(str.data()+lastPos,
                             (sizeof(T))*pos-lastPos ));
      }
      lastPos = pos + 1;
    }
  }

}

#endif

