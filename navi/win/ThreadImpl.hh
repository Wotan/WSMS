#ifndef _THREADIMPL_H_
# define _THREADIMPL_H_

# include <windows.h>
# include <WinBase.h>
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
	_thread = CreateThread(NULL, 0, callbackImpl, static_cast<void*>(helper), 0, NULL);
    return _thread  != 0 ? true : false;
  }


  bool join()
  {
	return WaitForSingleObject(_thread, INFINITE) == WAIT_FAILED ? false : true;
  }

  bool cancel()
  {
	DWORD lpExitCode;

	ExitThread(0);
    GetExitCodeThread(_thread, &lpExitCode);
	return lpExitCode == STILL_ACTIVE ? false : true;
  }

  char const* getError()
  {
    return "NOT IMPLEMENTED";
  }

private:
	static DWORD WINAPI callbackImpl(LPVOID arg)
  {
    CallbackHelper* helper = static_cast<CallbackHelper*>(arg);
    helper->callback(helper->arg);
    delete helper;
    return 0;
  }

private:
	HANDLE _thread;
	int _lastError;
};

} // !priv
} // !navi

#endif /* !_THREADIMPL_H_ */
