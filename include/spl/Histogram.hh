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

    template<typename V, typename L>
    struct BinHistogramBase
    {
      BinHistogramBase() : _max(0) {}
        void inc(V val_index, unsigned num=1)
        {
          (static_cast<L*>(this)->operator[](val_index)).inc(val_index, num);
          _max += num;
        }

        void dec(V val_index, unsigned num=1)
        {
          (static_cast<L*>(this)->operator[](val_index)).dec(val_index, num);
          _max -= num;
        }
       

        unsigned _max;
    };

    template<typename V, unsigned bin_size, unsigned N>
    struct BinHistogram<V, bin_size, N, true> : BinHistogramBase<V, BinHistogram<V, bin_size, N, true>>
    {
      typedef BinHistogramBase<V, BinHistogram<V, bin_size, N, true>> parent;

      BinHistogram(V start_index=0)
      : parent()
      , _start_index(start_index)
      , _layers(1<<bin_size, NULL)
      {}

      void operator-=(const BinHistogram<V, bin_size, N, true>& c)
      {
        for(unsigned i=0; i < _layers.size(); ++i)//TODO: memory alloc
          _layers[i] -= c->_layers[i];
        parent::_max -= c->_max;
      }

      void operator+=(const BinHistogram<V, bin_size, N, true>& c)
      {
        for(unsigned i=0; i < _layers.size(); ++i)//TODO: memory alloc
          _layers[i] += c->_layers[i];
        parent::_max += c->_max;
      }
      void clear()
      {
        for(unsigned i=0; i < _layers.size(); ++i)
          if(_layers[i])
          {
            _layers[i]->clear();//TODO: clean useless leaf = 0
            delete _layers[i];
            _layers[i] = NULL;
          }
        parent::_max = 0 ;
      }

      BinHistogram<V,bin_size, N - bin_size, true>& operator[](V val_index)
      {
        unsigned val_bin = val_index / (1 << bin_size);
        if(!_layers[val_bin])
        {
          _layers[val_bin] = new BinHistogram<V,bin_size, N - bin_size,true>(_start_index+( val_bin * (1 << bin_size)));
        }

        return *_layers[val_bin];
      }

      V median() const
      {
        unsigned middle  = parent::_max / 2 + ((parent::_max % 2 == 0) ? 0 : 1);
        unsigned middle_bin = middle / (1 << bin_size);
        unsigned sub_middle = 0;
        for (unsigned i=0; i < middle_bin; ++i)
          sub_middle += _layers[i]->_max;

        return _layers[middle_bin]->nth_element(middle-sub_middle);

      }

      private:
      V nth_element(unsigned middle) const
      {
        unsigned middle_bin = middle / (1 << bin_size);
        unsigned sub_middle = 0;
        for (unsigned i=0; i < middle_bin; ++i)
          sub_middle += _layers[i]->_max;

        return _layers[middle_bin]->nth_element(middle-sub_middle);
      }

      V _start_index;
      std::vector<BinHistogram<V,bin_size, N - bin_size, true>*> _layers;

      template<typename a, unsigned b, unsigned c, bool d>
      friend struct BinHistogram;
    };

    template<typename V, unsigned bin_size, unsigned N>
    struct BinHistogram<V, bin_size, N, false> : BinHistogramBase<V, BinHistogram<V, bin_size, N, false>>
    {

      typedef BinHistogramBase<V, BinHistogram<V, bin_size, N, false>> parent;

      BinHistogram(V start_index=0)
      : parent()
      , _start_index(start_index)
      , _layers()
      {
        for(unsigned i=0; i < (1 << bin_size); ++i)
          _layers.push_back(i*((1 << bin_size))+_start_index);
      }


      BinHistogram<V,bin_size, N - bin_size, false>& operator[](V val_index)
      {
        return _layers[val_index / (1 << bin_size)];
      }

      void operator-=(const BinHistogram<V, bin_size, N, false>& c)
      {
        for(unsigned i=0; i < _layers.size(); ++i)
          _layers[i] -= c._layers[i];
        parent::_max -= c._max;
      }

      void operator+=(const BinHistogram<V, bin_size, N, false>& c)
      {
        for(unsigned i=0; i < _layers.size(); ++i)
          _layers[i] += c._layers[i];
        parent::_max += c._max;
      }
      void clear()
      {
        for(unsigned i=0; i < _layers.size(); ++i)
          _layers[i].clear();
        parent::_max = 0 ;
      }

      V median() const
      {
        unsigned middle  = parent::_max / 2 + ((parent::_max % 2 == 0) ? 0 : 1);
        unsigned i = 0; //middle / (1 << bin_size);
        unsigned sub_middle = 0;
        while(sub_middle+_layers[i]._max < middle)
        {
          sub_middle += _layers[i]._max;
          ++i;
          assert(i < _layers.size());
        }
        return _layers[i].nth_element(middle-sub_middle);
      }

      private:
      V nth_element(unsigned middle) const
      {

        unsigned i = 0; //middle / (1 << bin_size);
        unsigned sub_middle = 0;
        while(sub_middle+_layers[i]._max < middle)
        {
          sub_middle += _layers[i]._max;
          ++i;
          assert(i < _layers.size());
        }
        return _layers[i].nth_element(middle-sub_middle);
      }

      private:
      V _start_index;
      std::vector<BinHistogram<V,bin_size, N - bin_size, false>> _layers;

      public:
      //unsigned _max;

      template<typename a, unsigned b, unsigned c, bool d>
      friend struct BinHistogram;

    };

    ///LEAVES

    namespace helper 
    {
      template<typename V, unsigned bin_size>
      struct BinHistogram
      {

        BinHistogram(V start_index=0)
        : _start_index(start_index)
        , _final_layer(1 << bin_size, 0)
        , _max(0)
        {}

        void inc(V val_index, unsigned num=1)
        {
          (*this)[val_index] += num;
          _max += num;
        }

        void dec(V val_index, unsigned num=1)
        {
          (*this)[val_index] -= num;
          _max -= num;
        }

      void operator-=(const BinHistogram<V, bin_size>& c)
      {
        for(unsigned i=0; i < _final_layer.size(); ++i)
          _final_layer[i] -= c._final_layer[i];
        _max -= c._max;
      }

      void operator+=(const BinHistogram<V, bin_size>& c)
      {
        for(unsigned i=0; i < _final_layer.size(); ++i)
          _final_layer[i] += c._final_layer[i];
        _max += c._max;
      }

      void clear()
      {
        for(unsigned i=0; i < _final_layer.size(); ++i)
          _final_layer[i] = 0;
        _max =0;
      }

        V median() const
        {
          unsigned i = 0;
          assert(i < _final_layer.size());
          unsigned counter = _final_layer[i];
          unsigned middle  = _max / 2 + ((_max % 2 == 0) ? 0 : 1);
          
          while(counter < middle)
          {
            ++i;
            assert(i < _final_layer.size());
            counter += _final_layer[i];
          }
          return i;
        }

        V nth_element(unsigned middle) const
        {
          unsigned i=0, sum = _final_layer[0];
          //middle -= _final_layer[i];
          while(sum <= middle)
          {
            ++i;
            assert(i < _final_layer.size());
            sum += _final_layer[i];
          }
          return _start_index+i;
        }



        protected:
        unsigned& operator[](V val_index)
        {
          return _final_layer[val_index-_start_index];
        }
        V _start_index;
        std::vector<unsigned> _final_layer;

        public:
        unsigned _max;

        //template<typename a, unsigned b, unsigned c, bool d>
        //friend struct BinHistogram;

      };

    } //!helper

    template<typename V, unsigned bin_size>
    struct BinHistogram<V, bin_size, 0, false> : helper::BinHistogram<V, bin_size>
    {
      BinHistogram(V start_index=0) :  helper::BinHistogram<V, bin_size>(start_index) {}
    };

    template<typename V, unsigned bin_size>
    struct BinHistogram<V, bin_size, 0, true> : helper::BinHistogram<V, bin_size>
    {
      BinHistogram(V start_index=0) :  helper::BinHistogram<V, bin_size>(start_index) {}
    };

  }

# endif

