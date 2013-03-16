#ifndef _EXCEPTION_H_
# define _EXCEPTION_H_

# include <exception>
# include <stdexcept>
# include <cstdio>
# include <cstdarg>

# include "Navi.hh"

namespace navi {

class Exception : public std::exception {
  static int const buffer_size = 512;
public:
  Exception(std::string const& str) : _str(str) {}
  Exception(char const* str, ...) : _str() {
    va_list ap;
    va_start(ap, str);
    setStringVaArgs(str, ap);
    va_end(ap);
  }
  void setStringVaArgs(char const* str, va_list ap) {
    char buffer[buffer_size];
#if defined(NAVI_WIN_PLATEFORM)
    vsnprintf_s(buffer, buffer_size, str, ap);
#else
    vsnprintf(buffer, buffer_size, str, ap);
#endif
    _str.append(buffer);
  }
  Exception& operator=(exception const& o);
  virtual ~Exception() throw() {}
  virtual const char* what() const throw() { return _str.c_str(); }
protected:
  Exception() : _str("") {}
private:
  std::string _str;
};

# define DEFINE_EXCEPTION(NAME)					\
  class NAME : public navi::Exception {				\
  public:							\
  NAME(std::string const& str) : navi::Exception(str) {}	\
  NAME(char const* str, ...) : navi::Exception() {		\
    va_list ap;							\
    va_start(ap, str);						\
    setStringVaArgs(str, ap);					\
    va_end(ap);							\
  }								\
  }

DEFINE_EXCEPTION(Error);

} // !navi



#endif /* !_EXCEPTION_H_ */
