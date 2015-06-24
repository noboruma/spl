#ifndef SPL_IO_HH
#define SPL_IO_HH

#include <string>
#include <fstream>
#include <iostream>

namespace spl 
{
  namespace io 
  {
    namespace csv 
    {

      template<typename S>
      void save(const std::string& file, const NDSignal<S>& in);

      template<typename S>
      void load(const std::string& file, NDSignal<S>& out);

    } //!csv

    namespace raw 
    {
      template<typename S>
      void save(const std::string& file, const NDSignal<S>& in);

      template<typename S>
      void load(const std::string& file, NDSignal<S>& out);
    } //!raw
  } //!io
} //!spl

#include "io.hxx"

#endif
