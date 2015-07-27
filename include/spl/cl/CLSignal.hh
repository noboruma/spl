#ifndef SPL_CL_SIGNAL_HH
#define SPL_CL_SIGNAL_HH

# include <CL/cl.hpp>
#include <string>
#include <iostream>

namespace spl 
{
  namespace cl 
  {
    template<typename V>
    struct CLSignal
    {
      CLSignal(const ::cl::Device &device, V* data, const size_t data_size);


      void convolution(const std::string &kernel, std::vector<std::vector<V> >& buffers);

      template< typename ... Args>
      void convolution(const std::string &kernel, Args ... args);

      protected:
        const ::cl::Device &_device;
        ::cl::Context _context;
        ::cl::Program::Sources _sources;
        ::cl::Buffer _cl_data;
        V *_rdata;
        const size_t _data_size;
        ::cl::Program _program;
    };
    
  } //!cl
  
} //!spl

#include "CLSignal.hxx"

#endif
