#ifndef SPL_CL_EXTERNAL_PROCESS_HH
#define SPL_CL_EXTERNAL_PROCESS_HH

#include "metatools.hh"
#include <array>

namespace spl 
{
  namespace cl 
  {
    template<typename ... SignalTypes>
    struct ExternalProcess()
    {
      ExternalProcess(SignalTypes&&... sigs)
      {
        spl::ct::apply_for([](unsigned i, auto& s)
                            {_cl_buffers[i] = ::cl::Buffer(_context), CL_MEM_READ_WRITE, sizeof(V)*s.size()},
                            sigs...);
      }

      void operator()()
      {
      }

      private:
      std::array<::cl::Buffer, sizeof...(SignalTypes)> _cl_buffers;


    };
    
  } //!cl
} //!spl
#endif
