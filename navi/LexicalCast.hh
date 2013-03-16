#ifndef _LEXICALCAST_H_
# define _LEXICALCAST_H_

# include <sstream>
# include <exception>

namespace navi {

class BadLexicalCast : public std::bad_cast {
public:
  virtual char const* what() const throw() { return "Bad lexical cast"; }
};

template <typename T>
std::string LexicalCast(T const& t)
{
  std::ostringstream ss;
  ss << t;
  if (!ss)
    throw BadLexicalCast();
  return ss.str();
}

template <typename T>
T LexicalCast(std::string const& str)
{
  std::istringstream ss(str);
  T t;
  ss >> t;
  if (!ss || ss.good())
    throw BadLexicalCast();
  return t;
}

} // !navi

#endif /* !_LEXICALCAST_H_ */
