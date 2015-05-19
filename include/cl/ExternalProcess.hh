#ifndef SPL_CL_EXTERNAL_PROCESS_HH
#define SPL_CL_EXTERNAL_PROCESS_HH

#include "metatools.hh"
#include "../NDSignal.hh"
#include <array>

namespace spl 
{
  namespace cl 
  {
    
    template<typename ... SignalTypes>
    struct ExternalProcess
    {
      // =====================================================================
      ExternalProcess(::cl::Device device,
                      const std::string &sources,
                      SignalTypes&... sigs)
      : _device(device)
      , _context({device})
      , _inputs({sigs...})
      , _queue(_context,_device)
      {

        _sources.push_back({sources.c_str(),sources.length()});
        _program = {_context,_sources};
        if(_program.build({_device})!=CL_SUCCESS)
          throw std::logic_error(std::string("Build error: ")+
                                 _program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_device)+
                                 "\n");

        spl::ct::if_apply_for<std::is_const>([=](unsigned i, auto s)
                                             {
                                             typedef typename std::remove_reference<decltype(s)> SignalType;
                                             _cl_buffers[i] = ::cl::Buffer(_context,
                                                                           CL_MEM_READ_WRITE,
                                                                           sizeof(traits_value_type(decltype(s)))*s.domain().prod());
                                             },
                                             [=](unsigned i, auto s)
                                             {
                                             _cl_buffers[i] = ::cl::Buffer(_context,
                                                                           CL_MEM_READ_WRITE,
                                                                           sizeof(traits_value_type(decltype(s)))*s.domain().prod());
                                             },
                                             sigs...);
      }

      // =====================================================================
      void push()
      {
        ct::apply([=](auto s) 
        {
          _queue.enqueueWriteBuffer(_cl_buffers[0],
                                    CL_TRUE,
                                    0,
                                    sizeof(traits_value_type(decltype(s)))*s.domain().prod(),
                                    (traits_value_type(decltype(s))*)s);
        }, _inputs);
      }
      
      // =====================================================================
      //template<unsigned ...I>
      void pull()
      {
        ct::apply([=](auto s) 
        {
          _queue.enqueueReadBuffer(_cl_buffers[0],
                                   CL_TRUE,
                                   0,
                                   sizeof(traits_value_type(decltype(s)))*s.domain().prod(),
                                   (traits_value_type(decltype(s))*)s);
        }, _inputs);
      }

      // =====================================================================
      void operator()(::cl::NDRange _range,const std::string& kernel_name="main")
      {
      //  ::cl::make_kernel<::cl::Buffer> convol(::cl::Kernel(_program,kernel_name.c_str()));

      //  //size_t arg_id = 0;
      //  //for(auto& cl_buffer : _cl_buffers)
      //    //convol.setArg(arg_id++,cl_buffer);

      //  ::cl::EnqueueArgs eargs(_queue,
      //                          ::cl::NullRange,
      //                          _range,
      //                          ::cl::NullRange);
      //  convol(eargs, _cl_buffers[0]).wait();

      ::cl::Kernel convol = ::cl::Kernel(_program,"main");

      size_t arg_id = 0;
      for(auto& cl_buffer : _cl_buffers)
        convol.setArg(arg_id++,cl_buffer);
      _queue.flush();

      _queue.enqueueNDRangeKernel(convol,
                                 ::cl::NullRange,
                                 _range, 
                                 ::cl::NullRange);
      }

      // =====================================================================
      //private:
      std::array<::cl::Buffer, sizeof...(SignalTypes)> _cl_buffers;

      const ::cl::Device &_device;
      ::cl::Context _context;
      ::cl::Program::Sources _sources;
      ::cl::Program _program;
      std::tuple<SignalTypes...> _inputs ;
      ::cl::CommandQueue _queue;


    };
    
  } //!cl
} //!spl
#endif
