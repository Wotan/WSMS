#ifndef _MUTEXIMPL_H_
# define _MUTEXIMPL_H_

# include <cassert>
# include <pthread.h>
# include <errno.h>

# include "navi/Exception.hh"

namespace navi {

class Mutex {
public:
  Mutex() {
    if (pthread_mutex_init(&_mutex, 0) != 0)
      throw navi::Error("Unable to init mutex");
  }

  void lock() {
    if (pthread_mutex_lock(&_mutex) != 0)
      throw navi::Error("Unable to lock mutex");
  }

  void unlock() {
    if (pthread_mutex_unlock(&_mutex) != 0)
      throw navi::Error("Unable to unlock mutex");
  }

  bool tryLock() {
    int res = pthread_mutex_trylock(&_mutex);
    if (res != 0 && res != EBUSY)
      throw navi::Error("Unable to trylock mutex");
    return !res;
  }

  pthread_mutex_t* getHandle() { return &_mutex; }

  virtual ~Mutex() {
    assert(pthread_mutex_destroy(&_mutex) == 0);
  }
private:
  pthread_mutex_t _mutex;
};

} //navi

#endif /* !_MUTEXIMPL_H_ */
