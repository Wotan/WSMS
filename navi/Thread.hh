#ifndef _THREAD_H_
# define _THREAD_H_

# include "Navi.hh"
# include "Bind.hh"
# include "Exception.hh"

# include <iostream>

# if defined(NAVI_UNIX_PLATEFORM) || defined(NAVI_MAC_PLATEFORM)
#  include "unix/ThreadImpl.hh"
# elif defined(NAVI_WIN_PLATEFORM)
#  include "win/ThreadImpl.hh"
# else
#  error "Navi thread: plateform not supported"
# endif

namespace navi {

class Thread {
  DEFINE_EXCEPTION(Error);
  typedef void (*CallbackType)(void*);
public:
  virtual ~Thread() {
    if (isJoinable())
      join();
  }

  template <typename FUN>
  Thread(FUN fun)
  {
    startThread(bind(fun));
  }

  template <typename FUN, typename A1>
  Thread(FUN fun, A1 a1)
  {
    startThread(bind(fun, a1));
  }

  template <typename FUN, typename A1, typename A2>
  Thread(FUN fun, A1 a1, A2 a2)
  {
    startThread(bind(fun, a1, a2));
  }

  template <typename FUN, typename A1, typename A2, typename A3>
  Thread(FUN fun, A1 a1, A2 a2, A3 a3)
  {
    startThread(bind(fun, a1, a2, a3));
  }

  template <typename FUN, typename A1, typename A2, typename A3, typename A4>
  Thread(FUN fun, A1 a1, A2 a2, A3 a3, A4 a4)
  {
    startThread(bind(fun, a1, a2, a3, a4));
  }

  template <typename FUN, typename A1, typename A2, typename A3, typename A4, typename A5>
  Thread(FUN fun, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
  {
    startThread(bind(fun, a1, a2, a3, a4, a5));
  }

  template <typename FUN, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  Thread(FUN fun, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
  {
    startThread(bind(fun, a1, a2, a3, a4, a5, a6));
  }

  bool isJoinable() const {
    return _started;
  }

  void join()
  {
    if (!_impl.join())
      threadError();
    _started = false;
  }

  void cancel()
  {
    if (!_impl.cancel())
      threadError();
    _started = false;
  }

private:
  template <typename BinderType>
  void startThread(BinderType binder)
  {
    _started = true;
    BinderType* b = new BinderType(binder);
    if (!_impl.create(&ThreadCallback<BinderType>, b))
      threadError();
  }

  void threadError()
  {
    throw Error("Thread error: %s", _impl.getError());
  }

  template <typename BinderType>
  static void ThreadCallback(void *arg)
  {
    BinderType* binder = static_cast<BinderType*>(arg);
    (*binder)();
    delete binder;
  }

private:
  priv::ThreadImpl _impl;
  bool _started;
};

} // !navi

#endif /* !_THREAD_H_ */
