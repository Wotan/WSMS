#ifndef _THREADGROUP_H_
# define _THREADGROUP_H_

# include <list>
# include <algorithm>

# include "Lock.hh"
# include "Thread.hh"

namespace navi {

class ThreadGroup : public ObjectLevelLockable<ThreadGroup> {
  typedef std::list<Thread*> ThreadList;
public:

  virtual ~ThreadGroup() {
    for (ThreadList::iterator it = _threads.begin();
	 it != _threads.end(); ++it) {
      delete *it;
    }
  }

  void addThread(Thread* t) {
    Lock(this);
    if (t)
      _threads.push_back(t);
  }

  void removeThread(Thread* t) {
    Lock(this);
    std::remove(_threads.begin(), _threads.end(), t);
  }

  void joinAll() {
    Lock(this);
    for (ThreadList::iterator it = _threads.begin();
	 it != _threads.end(); ++it) {
      (*it)->join();
    }
  }

  void cancelAll() {
    Lock(this);
    for (ThreadList::iterator it = _threads.begin();
	 it != _threads.end(); ++it) {
      (*it)->cancel();
    }
  }

private:
  ThreadList _threads;
};

} // !navi

#endif /* !_THREADGROUP_H_ */








