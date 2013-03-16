#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

# include "Mutex.hh"
# include "ConditionVariable.hh"
# include "Lock.hh"

namespace navi {

class Semaphore {
public:
  Semaphore(int value = 0)
  : _value(value) {
  }
  virtual ~Semaphore() {}

  int getValue() const {
    ScopedLock guard(_mutex);
    return _value;
  }

  void post() {
    ScopedLock guard(_mutex);
    _value++;
    _condition.notify();
  }

  void wait() {
    ScopedLock guard(_mutex);
    while (!_value)
      _condition.wait(_mutex);
    _value--;
  }

private:
  mutable Mutex _mutex;
  ConditionVariable _condition;
  int _value;
};

} // !navi

#endif /* !_SEMAPHORE_H_ */
