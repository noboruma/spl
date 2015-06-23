#include <tuple>
namespace spl 
{
  namespace cl 
  {
    template<typename V>
    CLSignal<V>::CLSignal(const ::cl::Device &device, V* data, size_t data_size)
    : _device(device)
    , _context({device})
    , _cl_data(_context, CL_MEM_READ_WRITE,sizeof(V)*data_size)
    , _rdata(data)
    , _data_size(data_size)
    {}

    void variadic_enqueue()
    {
    }
    template<typename t, typename... Args>
    void variadic_enqueue(t e, Args... args)
    {

      std::cout<<e<<std::endl;
      //queue.enqueuewritebuffer(e,cl_true,0,sizeof(v)*e.size(),e);
      variadic_enqueue(args...);
    }

    template<typename V>
    void CLSignal<V>::convolution(const std::string &kernel, std::vector<std::vector<V> > &buffers)
    {
      std::vector<::cl::Buffer> cl_buffers;
      variadic_enqueue(1,2, 2.,3);

      for(auto& buffer : buffers)
        cl_buffers.push_back({_context, CL_MEM_READ_ONLY, sizeof(V)*buffer.size()});

      _sources.push_back({kernel.c_str(),kernel.length()});
      ::cl::Program program(_context,_sources);
      if(program.build({_device})!=CL_SUCCESS)
        throw std::logic_error(std::string("Build error: ")+
                               program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_device)+
                               "\n");


      ::cl::CommandQueue queue(_context,_device);
      queue.enqueueWriteBuffer(_cl_data,
                               CL_TRUE,
                               0,
                               sizeof(V)*_data_size,
                               _rdata);

      for(size_t i = 0; i < cl_buffers.size(); ++i)
        queue.enqueueWriteBuffer(cl_buffers[i],
                                 CL_TRUE,
                                 0,
                                 sizeof(V)*buffers[i].size(),
                                 buffers[i].data());

      ::cl::Kernel convol = ::cl::Kernel(program,"main");

      size_t arg_id = 0;
      convol.setArg(arg_id++,_cl_data);

      for(auto& cl_buffer : cl_buffers)
        convol.setArg(arg_id++,cl_buffer);
      queue.flush();

      queue.enqueueNDRangeKernel(convol,
                                 ::cl::NullRange,
                                 ::cl::NDRange(_data_size),
                                 ::cl::NullRange);

      queue.finish();
      queue.enqueueReadBuffer(_cl_data,
                              CL_TRUE,
                              0,
                              sizeof(V)*_data_size,
                              _rdata);
    }


    template<typename V>
    template<typename ... Args>
    void CLSignal<V>::convolution(const std::string &kernel, Args... args)
    {
      _sources.push_back({kernel.c_str(),kernel.length()});
      ::cl::Program program(_context,_sources);
      if(program.build({_device})!=CL_SUCCESS)
        throw std::logic_error(std::string("Build error: ")+
                               program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_device)+
                               "\n");
      ::cl::CommandQueue queue(_context,_device);

      queue.enqueueWriteBuffer(_cl_data,CL_TRUE,0,sizeof(V)*_data_size,_rdata);

      ::cl::make_kernel<::cl::Buffer&, Args...>convol(::cl::Kernel(program,"main"));
      ::cl::EnqueueArgs eargs(queue,
                              ::cl::NullRange,
                              ::cl::NDRange(_data_size),
                              ::cl::NullRange);

      convol(eargs,_cl_data, args...).wait();

      queue.enqueueReadBuffer(_cl_data,
                              CL_TRUE,
                              0,
                              sizeof(V)*_data_size,
                              _rdata);
    }
  } //!cl
} //!spl
