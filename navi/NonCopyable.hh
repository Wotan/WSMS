#ifndef _NONCOPYABLE_H_
# define _NONCOPYABLE_H_

namespace navi {

class NonCopyable {
protected:
  NonCopyable() {}
  virtual ~NonCopyable() {}
private:
  NonCopyable(NonCopyable const&);
  NonCopyable& operator=(NonCopyable const&);
};

} // !navi

#endif /* !_NONCOPYABLE_H_ */
