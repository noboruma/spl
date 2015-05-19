// vim: set nofoldenable :
// vim: set fdm=marker fmr={{{,}}} :
// vim: set foldlevel=0 :
// vim: set foldenable :

#include "cl/2DSignal.hh"
#include "cl/ExternalProcess.hh"
#include <iostream>
#include <assert.h>
#include <fstream>

int main()
{
  // 2D OpenCL Signal
  {
    //{{{
    std::vector<::cl::Platform> all_platforms;
    ::cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0)
    {
      std::cout<<" No platforms found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<::cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0)
    {
      std::cout<<" No devices found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
    //}}}

    spl::cl::Signal2D<float> ss(10,10,default_device);

    std::ifstream f("kernels/set210.cl");
    std::string kernel((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
    f.close();
    ss.convolution(kernel);

    for_each_pixels(ss,x,y)
    {
      assert(ss(x,y) == 10);
      std::cout<<ss(x,y)<<std::endl;
    }
  }

  // Multiple OpenCL Signal convolution
  {
//{{{
    std::vector<::cl::Platform> all_platforms;
    ::cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0)
    {
      std::cout<<" No platforms found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<::cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0)
    {
      std::cout<<" No devices found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
//}}}


    spl::cl::Signal2D<float> ss(10,1,default_device);

    std::vector<float> B = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<float> C = {9, 8, 7, 6, 5, 4 ,3 ,2 ,1, 0};


    std::vector<std::vector<float>> buffs;
    buffs.push_back(B);
    buffs.push_back(C);

    std::ifstream f("kernels/add2vectors.cl");
    std::string kernel((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
    f.close();
    ss.convolution(kernel, buffs);
    for_each_pixels(ss,x,y)
    {
      assert(ss(x,y) == 9);
      std::cout<<ss(x,y)<<std::endl;
    }
  }

  // External Process
  {
//{{{
    std::vector<::cl::Platform> all_platforms;
    ::cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0)
    {
      std::cout<<" No platforms found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<::cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0)
    {
      std::cout<<" No devices found. Check OpenCL installation!\n";
      assert(false);
    }
    ::cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
//}}}
  

    std::ifstream f("kernels/set210.cl");
    std::string kernel((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
    f.close();
    spl::Signal2D<float> ss(10,1);
    spl::cl::ExternalProcess<spl::Signal2D<float>> ep(default_device,kernel,ss);
    ep.push();
    ep(::cl::NDRange(10,1));
    ep.pull();

    for_each_pixels(std::get<0>(ep._inputs),x,y)
    {
      assert(ss(x,y) == 10);
      std::cout<<ss(x,y)<<std::endl;
    }
  }

}
