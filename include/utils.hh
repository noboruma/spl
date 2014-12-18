# ifndef SPL_UTILS_HH
# define SPL_UTILS_HH

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

void LOG(const std::string& s, const std::string& tag = "NIA")
{
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[20];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

  std::cout<<"["<<tag<<"] "<<buf<<":"<< s <<std::endl;
}

void LOG_ERR(const std::string& s, const std::string& tag = "NIA")
{
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[20];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

  std::cerr<<"["<<tag<<"] "<<buf<<":"<< s <<" at "<<__FILE__<<": "<<__LINE__<<std::endl;
}

# endif
