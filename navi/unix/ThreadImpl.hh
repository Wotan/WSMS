#ifndef _THREADIMPL_H_
# define _THREADIMPL_H_

# include <pthread.h>
# include <cstring>

namespace navi {
namespace priv {

class ThreadImpl
{
  typedef void (*CallbackType)(void*);
  struct CallbackHelper {
    void *arg;
    CallbackType callback;
  };

public:
  ThreadImpl() : _lastError(0) {}
  bool create(CallbackType callback, void *arg)
  {
    CallbackHelper* helper = new CallbackHelper;
    helper->arg = arg;
    helper->callback = callback;
    _lastError = pthread_create(&_thread, 0, &callbackImpl, static_cast<void*>(helper));
    return _lastError == 0 ? true : false;
  }

  bool join()
  {
    _lastError = pthread_join(_thread, 0);
    return _lastError == 0 ? true : false;
  }

  bool cancel()
  {
    _lastError = pthread_cancel(_thread);
    return _lastError == 0 ? true : false;
  }

  char const* getError()
  {
    return strerror(_lastError);
  }

private:
  static void *callbackImpl(void *arg)
  {
    CallbackHelper* helper = static_cast<CallbackHelper*>(arg);
    helper->callback(helper->arg);
    delete helper;
    return 0;
  }

private:
  pthread_t _thread;
  int _lastError;
};

} // !priv
} // !navi

#endif /* !_THREADIMPL_H_ */
