#ifndef _MUTEXIMPL_H_
# define _MUTEXIMPL_H_

# include <Windows.h>
# include "navi/Exception.hh"

namespace navi {

class Mutex {
public:
  Mutex() {
	  _mutex = CreateMutex(NULL, FALSE, NULL);
	  if (_mutex == NULL)
		  throw navi::Error("Unable to init Mutex");
  }

  void lock() {
	  if (WaitForSingleObject(_mutex, INFINITE) == WAIT_FAILED)
		  throw navi::Error("Unable to lock mutex");
  }

  void unlock() {
	if (!ReleaseMutex(_mutex))
		throw navi::Error("Unable to unlock mutex");
  }

  bool tryLock() {
  DWORD ret;

 ret = WaitForSingleObject(_mutex, 0);
  if (ret == WAIT_TIMEOUT)
	  return false;
  if (ret == WAIT_FAILED)
	  throw navi::Error("Unable to trylock mutex");
  else
	  return true;
  }

  virtual ~Mutex() {
	  CloseHandle(_mutex);
  }
private:
	HANDLE _mutex;
};


} //navi

#endif /* !_MUTEXIMPL_H_ */
