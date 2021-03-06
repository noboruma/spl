  namespace spl{

    // =====================================================================
    template<typename E, unsigned dim>
    Point<E,dim>::Point() {}

    // =====================================================================
    template<typename E, unsigned dim>
    const unsigned &Point<E,dim>::operator[](unsigned i) const 
    {
      assert(i < dim);
      return _coords[i];
    }

    // =====================================================================
    template<typename E, unsigned dim>
    unsigned &Point<E,dim>::operator[](unsigned i) 
    {
      assert(i < dim);
      return _coords[i];
    }

    // =====================================================================
    template<typename E, unsigned dim>
    E Point<E,dim>::operator+(const E &p) const
    {
      E ret;
      for(unsigned i=0; i < dim; ++i)
        ret[i] = (*this)[i] + p[i];
      return ret;
    }

    // =====================================================================
    template<typename E, unsigned dim>
    E& Point<E,dim>::operator+=(const E &p)
    {
      this->exact() = this->exact() + p;
      return this->exact();
    }

    // =====================================================================
    template<typename E, unsigned dim>
    E Point<E,dim>::operator-(const E &p) const
    {
      E ret;
      for(unsigned i=0; i < dim; ++i)
        ret[i] = (*this)[i] - p[i];
      return ret;
    }

    // =====================================================================
    template<typename E, unsigned dim>
    E& Point<E,dim>::operator-=(const E &p)
    {
      this->exact() = this->exact() - p;
      return this->exact();
    }

    // =====================================================================
    template<typename E, unsigned dim>
    void Point<E,dim>::operator=(const E &p) 
    {
      for(unsigned i=0; i < dim; ++i)
        (*this)[i] = p[i];
    }

    // =====================================================================
    template<typename E, unsigned dim>
    Point<E,dim>::Point(const Point<E,dim>& p)
    {
      for(unsigned i=0; i < dim; ++i)
        (*this)[i] = p[i];
    }

    // =====================================================================
    template<typename E, unsigned dim>
    bool Point<E,dim>::operator==(const E &p) const
    {
      bool res=true;
      for(unsigned i=0; i < dim && res; ++i)
        res = _coords[i] == p._coords[i];
      return res;
    }
  }//!spl
