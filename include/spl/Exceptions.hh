#ifndef SPL_EXCEPTIONS_HH
#define SPL_EXCEPTIONS_HH

#include <stdexcept>

#define throw_logic(msg) \
    throw except::logic_exception(msg ,__FILE__,__LINE__)

#define throw_runtime(msg) \
    throw except::runtime_exception(msg ,__FILE__,__LINE__)

// TODO: 
//Qt has caught an exception thrown from an event handler. Throwing
//exceptions from an event handler is not supported in Qt. You must
//reimplement QApplication::notify() and catch all exceptions there.
// 
namespace except
{

class runtime_exception : public std::runtime_error 
{
  std::string msg;
  public:
  runtime_exception(const std::string &arg, const char *file, int line) 
  : std::runtime_error(arg) 
  {
    std::ostringstream o;
    o << file << ":" << line << " -> " << arg;
    msg = o.str();
  }

  ~runtime_exception() throw() 
  {}

  const char *what() const throw() 
  {
    return msg.c_str();
  }
};

class logic_exception : public std::logic_error 
{
  std::string msg;
  public:
  logic_exception(const std::string &arg, const char *file, int line) 
  : std::logic_error(arg) 
  {
    std::ostringstream o;
    o << file << ":" << line << " -> " << arg;
    msg = o.str();
  }

  ~logic_exception() throw() 
  {}

  const char *what() const throw() 
  {
    return msg.c_str();
  }
};

}

#endif
