#ifndef _CONDITIONVARIABLEIMPL_H_
#define _CONDITIONVARIABLEIMPL_H_

# include <pthread.h>
# include <cassert>

# include "../Exception.hh"
# include "../Mutex.hh"

namespace navi {

class ConditionVariable {
public:
  ConditionVariable() {
    if (pthread_cond_init(&_condition, 0))
      throw navi::Error("Unable to init condition variable");
  }

  virtual ~ConditionVariable() {
    assert(pthread_cond_destroy(&_condition) == 0);
  }

  void notify() {
    if (pthread_cond_signal(&_condition))
      throw navi::Error("Unable to notify condition variable");
  }

  void notifyAll() {
    if (pthread_cond_broadcast(&_condition))
      throw navi::Error("Unable to notify condition variable");
  }

  void wait(Mutex& m) {
    if (pthread_cond_wait(&_condition, m.getHandle()))
      throw navi::Error("Unable to wait condition variable");
  }

private:
  pthread_cond_t _condition;
};

} // !navi

#endif /* !_CONDITIONVARIABLEIMPL_H_ */





