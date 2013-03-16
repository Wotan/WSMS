#ifndef _ANY_H_
#define _ANY_H_

# include <typeinfo>
# include <exception>
# include <cassert>

namespace navi {

class BadAnyCast : public std::bad_cast {
public:
  virtual char const* what() const throw() { return "Bad any cast"; }
};

class Any {

  class HolderBase {
  public:
    virtual ~HolderBase() {}
    virtual HolderBase* clone() const = 0;
    virtual std::type_info const& getType() const = 0;
  };

  template <typename ValueType>
  class Holder : public HolderBase {
    friend class Any;
  public:
    Holder(ValueType const& value)
    : _value(value) {}
    HolderBase* clone() const { return new Holder(_value); }
    std::type_info const& getType() const { return typeid(ValueType); }
  private:
    ValueType _value;
  };

public:
  template <typename ValueType>
  Any(ValueType const& value)
  : _holder(new Holder<ValueType>(value))
  {
  }

  Any(Any const& any)
  : _holder(any._holder->clone())
  {
  }

  ~Any()
  {
    delete _holder;
  }

  inline Any& operator=(Any const& any)
  {
    delete _holder;
    _holder = any._holder->clone();
    return *this;
  }

  template <typename ValueType>
  inline Any& operator=(ValueType const& value)
  {
    get<ValueType>() = value;
    return *this;
  }

  template <typename ValueType>
  inline ValueType const* getPtr() const
  {
# ifndef NDEBUG
    if (_holder->getType() != typeid(ValueType))
      throw BadAnyCast();
# endif
    return &static_cast<Holder<ValueType>*>(_holder)->_value;
  }

  template <typename ValueType>
  inline ValueType* getPtr()
  {
    return const_cast<ValueType*>(static_cast<Any const&>(*this).getPtr<ValueType>());
  }

  template <typename ValueType>
  inline ValueType const& get() const
  {
    ValueType const* value = getPtr<ValueType>();
    return *value;
  }

  template <typename ValueType>
  inline ValueType& get()
  {
    return const_cast<ValueType&>(static_cast<Any const&>(*this).get<ValueType>());
  }


private:
  HolderBase* _holder;
};

} // !navi

#endif /* !_ANY_H_ */
