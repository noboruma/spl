# ifndef SPL_DPOINT_HH
# define SPL_DPOINT_HH

  namespace spl{
    /* **
     * DPoint structure is a convenient way to do a signal shift.
     */
    template<typename E, unsigned dim>
    struct DPoint
    {
      const signed &operator[](unsigned i) const;
      signed &operator[](unsigned i);

      E operator+(const E &p) const;
      E operator-(const E &p) const;

      void operator=(const E &p);

      protected:
      signed _coords[dim];
    };  

    struct DPoint1D : public DPoint<DPoint1D,1>
    {
      typedef DPoint<DPoint1D, 1> parent;

      DPoint1D(signed x) 
      : _x(_coords[0])
      {
        _x = x;
      }
      DPoint1D() 
      : _x(_coords[0])
      {
        _x = 0;
      }
      void operator=(const DPoint1D &p) 
      {
        parent::operator=(p);
      }
      signed &_x;
    };

    struct DPoint2D : public DPoint<DPoint2D,2>{
      typedef DPoint<DPoint2D, 2> parent;
      DPoint2D(signed x, signed y) 
      : _x(_coords[0])
        , _y(_coords[1])
      {
        _x = x;
        _y = y;
      }
      DPoint2D() 
      : _x(_coords[0])
        , _y(_coords[1])
      {
        _x = 0;
        _y = 0;
      }
      void operator=(const DPoint2D &p) 
      {
        parent::operator=(p);
      }

      signed &_x, &_y;
    };

    struct DPoint3D : public DPoint<DPoint3D,3>{
      typedef DPoint<DPoint3D, 3> parent;
      DPoint3D(signed x, signed y, signed z) 
      : _x(_coords[0])
      , _y(_coords[1])
      , _z(_coords[2])
      {
        _x = x;
        _y = y;
        _z = z;
      }
      DPoint3D() 
      : _x(_coords[0])
      , _y(_coords[1])
      , _z(_coords[2])
      {
        _x = 0;
        _y = 0;
        _z = 0;
      }
      void operator=(const DPoint3D &p) 
      {
        parent::operator=(p);
      }
      signed &_x, &_y, &_z;
    };
  }//!spl

#include "DPoint.hxx"

# endif
