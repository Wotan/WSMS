#ifndef _REF_H_
#define _REF_H_

namespace navi {

template<class T>
class ReferenceWrapper {
public:
  explicit ReferenceWrapper(T& t) : _p(&t) {}
  operator T&() const { return *_p; }
private:
  T* _p;
};

template<class T>
ReferenceWrapper<T> ref(T& t)
{
  return ReferenceWrapper<T>(t);
}

}

#endif /* !_REF_H_ */
