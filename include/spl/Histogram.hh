# ifndef SPL_SPL_HISTOGRAM_HH
# define SPL_SPL_HISTOGRAM_HH

#include <vector>
#include <unordered_map>
#include "2DSignal.hh"

  namespace spl
  {

    template<typename S>
    struct Histogram
    {
    
      Histogram(const S &sig)
      : _sig(sig)
      {}

      void operator()()
      {
        traits_iterator_type(S) it(_sig.domain());
        for_each_element(it)
          ++_res[_sig[it]];
      }

      std::unordered_map<traits_value_type(S), unsigned> _res;
      decltype(_res) &res(){return _res;}

      private:
      const S &_sig;

    };
  }//!spl
 
  namespace external
  {
    template<typename V, unsigned bin_size, unsigned N = (sizeof(V)*8) - bin_size, bool save_memory_space=false>
    struct BinHistogram;

    template<typename V, unsigned bin_size, unsigned N>
    struct BinHistogram<V, bin_size, N, true>
    {

      BinHistogram(V start_index=0)
      : _start_index(start_index)
        , _layers(1<<bin_size, NULL)
      {}

      unsigned& operator[](V val_index)
      {
        if(!_layers[val_index / (1 << bin_size)])
        {
          std::cout<< N<<std::endl;
          _layers[val_index / (1 << bin_size)] = new BinHistogram<V,bin_size, N - bin_size,true>(_start_index+( (val_index / (1 << bin_size)) * (1 << bin_size)));
        }

        return (*_layers[val_index / (1 << bin_size)])[val_index];
      }

      private:
      V _start_index;
      std::vector<BinHistogram<V,bin_size, N - bin_size, true>*> _layers;

    };

    template<typename V, unsigned bin_size, unsigned N>
    struct BinHistogram<V,bin_size,N,false>
    {

      BinHistogram(V start_index=0)
      : _start_index(start_index)
        , _layers()
      {
        for(unsigned i=0; i < (1 << bin_size); ++i)
          _layers.push_back(i*((1 << bin_size))+_start_index);
      }

      unsigned& operator[](V val_index)
      {
        return _layers[val_index / (1 << bin_size)][val_index];
      }

      private:
      V _start_index;
      std::vector<BinHistogram<V,bin_size, N - bin_size, false>> _layers;

    };

    namespace helper 
    {
      template<typename V, unsigned bin_size>
      struct BinHistogram
      {

        BinHistogram(V start_index=0)
        : _start_index(start_index)
          , _final_layer(bin_size)
        {}

        unsigned& operator[](V val_index)
        {
          return _final_layer[val_index-_start_index];
        }

        protected:

        V _start_index;
        std::vector<unsigned> _final_layer;

      };

    } //!helper

    template<typename V, unsigned bin_size>
    struct BinHistogram<V, bin_size, 0, false> : helper::BinHistogram<V, bin_size>
    {
      BinHistogram(V start_index) :  helper::BinHistogram<V, bin_size>(start_index) {}
    };

    template<typename V, unsigned bin_size>
    struct BinHistogram<V, bin_size, 0, true> : helper::BinHistogram<V, bin_size>
    {
      BinHistogram(V start_index) :  helper::BinHistogram<V, bin_size>(start_index) {}
    };

  }

# endif
