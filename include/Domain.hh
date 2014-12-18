# ifndef SPL_DOMAIN_HH
# define SPL_DOMAIN_HH

#include <type_traits>

  namespace spl
  {

    template<unsigned DIM>
    struct Domain
    {
      const static unsigned dim=DIM;

      unsigned bounds[DIM];

      Domain() 
      : bounds{0} {}

      //SFINAE principe
      template<typename T=unsigned[DIM]>
      Domain(unsigned r, typename std::enable_if<std::is_same<T, unsigned [1]>::value >::type* = 0) 
      : bounds{r} {}

      //SFINAE principe
      template<typename T=unsigned[DIM]>
      Domain(unsigned r, unsigned c, typename std::enable_if<std::is_same<T, unsigned [2]>::value>::type* = 0) 
      : bounds{r,c} {}

      //SFINAE principe
      template<typename T=unsigned[DIM]>
      Domain(unsigned r, unsigned c, unsigned d, typename std::enable_if<std::is_same<T, unsigned [3]>::value>::type* = 0) 
      : bounds{r,c,d} {}

      const unsigned& operator[](unsigned p) const {return bounds[p];};

      unsigned& operator[](unsigned p) {return bounds[p];};

      const unsigned prod() const
      {
        unsigned prod = bounds[0];
        for(unsigned i=1; i < DIM; ++i)
          prod *= bounds[i];
        return prod;
      }
    };

    /*template<>
    struct Domain<1>
    {
      const static unsigned dim=1;
      unsigned bounds[dim];
      Domain() 
      {bounds[0] = 0;}
      Domain(unsigned x) 
      {bounds[0] = x;}
      const unsigned& operator[](int p) const {return bounds[p];};
      unsigned& operator[](int p) {return bounds[p];};
    };
    template<>
    struct Domain<2>
    {
      const static unsigned dim=2;
      unsigned bounds[dim];
      Domain() 
      {bounds[0] = 0; bounds[1] = 0;};
      Domain(unsigned x, unsigned y) 
      {bounds[0] = x; bounds[1]=y;};
      const unsigned& operator[](int p) const {return bounds[p];};
      unsigned& operator[](int p) {return bounds[p];};
    };
    template<>
    struct Domain<3>
    {
      const static unsigned dim=3;
      unsigned bounds[dim];
      Domain() 
      {bounds[0] = 0; bounds[1]=0; bounds[2] = 0;};
      Domain(unsigned x, unsigned y, unsigned z) 
      {bounds[0] = x; bounds[1]=y; bounds[2] = z;};
      const unsigned& operator[](int p) const {return bounds[p];};
      unsigned& operator[](int p) {return bounds[p];};
    };*/
  }//!spl

# endif
