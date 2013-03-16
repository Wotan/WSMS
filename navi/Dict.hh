#ifndef _DICT_H_
# define _DICT_H_

# include <map>

# include "Any.hh"
# include "Exception.hh"

namespace navi {

class Dict {
public:
  DEFINE_EXCEPTION(Error);
  typedef std::map<std::string const, Any> AnyMap;
  typedef AnyMap::iterator iterator;
  typedef AnyMap::const_iterator const_iterator;

public:
  Dict() {}
  virtual ~Dict() {}

  Dict(Dict const& d) {
    *this = d;
  }

  Dict& operator=(Dict const& d)
  {
    _map = d._map;
    return *this;
  }

  template <typename T>
  Dict& set(std::string const& key, T const& value) {
    AnyMap::iterator it = _map.lower_bound(key);
    if (it != _map.end() && !(_map.key_comp()(key, it->first))) {
      it->second = value;
    } else {
      _map.insert(it, AnyMap::value_type(key, value));
    }
    return *this;
  }

  template <typename T>
  T const& get(std::string const& key) const {
    try {
      return (*this)[key].get<T>();
    } catch (BadAnyCast const&) {
      throw Error("navi::Dict: key '%s': invalid cast", key.c_str());
    }
  }

  template <typename T>
  T& get(std::string const& key) {
    return const_cast<T&>(static_cast<Dict const&>(*this).get<T>(key));
  }

  bool exist(std::string const& key) const
  {
    return _map.find(key) != _map.end();
  }

  std::size_t size() const
  {
    return _map.size();
  }

  navi::Any const& operator[](std::string const& str) const
  {
    try {
      return _map.at(str);
    } catch (std::exception&) {
      throw Error("navi::Dict - key \'%s\' not found", str.c_str());
    }
  }

  navi::Any& operator[](std::string const& str)
  {
    return const_cast<navi::Any&>(static_cast<Dict const&>(*this)[str]);
  }

  iterator begin()
  {
    return _map.begin();
  }

  iterator end()
  {
    return _map.end();
  }

  const_iterator begin() const
  {
    return _map.begin();
  }

  const_iterator end() const
  {
    return _map.end();
  }

private:
  AnyMap _map;
};

}

#endif /* !_DICT_H_ */
