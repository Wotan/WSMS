#ifndef _SPLIT_H_
# define _SPLIT_H_

# include <vector>

# include "boost/lexical_cast.hpp"

namespace navi {

template <typename T>
void split(std::string const& str, std::vector<T>& vect, char sep = ' ')
{
  for (std::size_t i = 0; i < str.size(); ++i) {
    for (;str[i] == sep && i < str.size(); ++i) ;
    if (str[i]) {
      std::string pending;
      for (;str[i] != sep && i < str.size(); ++i)
	pending.push_back(str[i]);
      vect.push_back(boost::lexical_cast<T, std::string>(pending));
    }
  }
}

} // !navi

#endif /* !_SPLIT_H_ */
