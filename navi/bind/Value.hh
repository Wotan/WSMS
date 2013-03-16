#ifndef _VALUE_H_
#define _VALUE_H_

# include "PlaceHolder.hh"

namespace navi {

template <typename T>
class Value {
public:
  Value(T&  t) : _t(t) {}
  T& get() { return _t; }
  T const& get() const { return _t; }

private:
  T _t;
};

template <typename T>
struct GetType
{
  typedef Value<T> Type;
};

template <int id>
struct GetType<Arg<id> >
{
  typedef Arg<id> Type;
};

} // !navi

#endif /* !_VALUE_H_ */
