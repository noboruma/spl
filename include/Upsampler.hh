#ifndef SPL_UPSAMPLER_HH
#define SPL_UPSAMPLER_HH

#include "2DSignal.hh"
#include "stdtools.hh"

  namespace spl
  {

    namespace ct
    {
      template<typename V, template<typename V, unsigned L> class interpolation, unsigned L>
      struct Up2DSampler
      {
        const Signal2D<V>& _sig;
        std::unique_ptr<Signal2D<V> > _up_sig;
        const Signal2D<V>& res(){ return (*_up_sig); }
        Up2DSampler(const Signal2D<V>& sig);
        void operator()();
      };

      template<typename V, template<typename V, unsigned L> class interpolation, unsigned L>
      struct Up2DSequenceSampler
      {
        const Signal3D<V> &_sig;
        std::unique_ptr<Signal3D<V> > _up_sig;
        const Signal3D<V> &res(){ return (*_up_sig); }
        Up2DSequenceSampler(const Signal3D<V>& sig);
        void operator()();
      };
    }//!ct

    namespace rt
    {
      template<typename V, template<typename V, unsigned L> class interpolation>
      struct Up2DSampler
      {
        const Signal2D<V>& res(){ return (*_up_sig); }
        Up2DSampler(const Signal2D<V>& sig, unsigned L_);
        void operator()();

        protected:
        const Signal2D<V>& _sig;
        std::unique_ptr<Signal2D<V> > _up_sig;
        const unsigned L;
      };

      template<typename V, template<typename V, unsigned L> class interpolation>
      struct Up2DSequenceSampler
      {
        const Signal3D<V> &res(){ return (*_up_sig); }
        Up2DSequenceSampler(const Signal3D<V>& sig, unsigned L_);
        void operator()();

        protected:
        const Signal3D<V> &_sig;      
        std::unique_ptr<Signal3D<V> > _up_sig;
        const unsigned L;
      };
    }//!rt
    
    // Interfaces
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L = 0>
    struct Up2DSampler : ct::Up2DSampler<V,interpolation, L>
    {
      typedef ct::Up2DSampler<V,interpolation, L> parent;
      Up2DSampler(const Signal2D<V>& sig)
      : parent(sig) {}
    };

    template<typename V, template<typename V, unsigned L> class interpolation>
    struct Up2DSampler<V, interpolation, 0> : rt::Up2DSampler<V,interpolation>
    {
      typedef rt::Up2DSampler<V,interpolation> parent;
      Up2DSampler(const Signal2D<V>& sig, unsigned L_)
      : parent(sig, L_) {}
    };

    // Interfaces
    template<typename V, template<typename V, unsigned L> class interpolation,unsigned L = 0>
    struct Up2DSequenceSampler : ct::Up2DSequenceSampler<V,interpolation,L>
    {
      typedef ct::Up2DSequenceSampler<V,interpolation, L> parent;
      Up2DSequenceSampler(const Signal3D<V>& sig)
      : parent(sig) {}
    };

    template<typename V, template<typename V, unsigned L> class interpolation>
    struct Up2DSequenceSampler<V, interpolation, 0> : rt::Up2DSequenceSampler<V,interpolation>
    {
      typedef rt::Up2DSequenceSampler<V,interpolation> parent;
      Up2DSequenceSampler(const Signal3D<V>& sig, unsigned L_)
      : parent(sig, L_) {}
    };

  }//!spl

#include "Upsampler.hxx"

#endif
