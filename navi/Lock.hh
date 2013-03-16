#ifndef _LOCK_H_
# define _LOCK_H_

# include <navi/Mutex.hh>

namespace navi {

class ScopedLock {
public:
  ScopedLock(Mutex& mutex) : _mutex(mutex) { _mutex.lock(); }
  ~ScopedLock() { _mutex.unlock(); }

private:
  Mutex& _mutex;
};

template <class T>
struct ObjectLevelLockable {
public:
  ObjectLevelLockable()
    : _mutex() {}
  ObjectLevelLockable(const ObjectLevelLockable&)
    : _mutex() {}
  ~ObjectLevelLockable() {}

  class Lock;
  friend class Lock;

  class Lock {
  public:
    explicit Lock(ObjectLevelLockable const* host)
      : _host(*host)
    {
      _host._mutex.lock();
    }
    explicit Lock(ObjectLevelLockable const& host)
      : _host(host)
    {
      _host._mutex.lock();
    }
    ~Lock() {
      _host._mutex.unlock();
    }
  private:
    Lock();
    Lock(const Lock&);
    Lock& operator=(const Lock&);
    ObjectLevelLockable const& _host;
  };

  class Unlock {
  public:
    explicit Unlock(ObjectLevelLockable const* host)
      : _host(*host)
    {
      _host._mutex.unlock();
    }
    explicit Unlock(ObjectLevelLockable const& host)
      : _host(host)
    {
      _host._mutex.unlock();
    }
    ~Unlock() {
      _host._mutex.lock();
    }
  private:
    Unlock();
    Unlock(const Unlock&);
    Unlock& operator=(const Unlock&);
    ObjectLevelLockable const& _host;
  };
  void lock() { _mutex.lock(); }
  void unlock() { _mutex.unlock(); }
  typedef volatile T VolatileType;
private:
  mutable Mutex _mutex;
};

template <class T>
class ClassLevelLockable {
public:
  class Lock {
  public:
    Lock()
    {
      _mutex.lock();
    }
    ~Lock() {
      _mutex.unlock();
    }
  private:
    Lock(const Lock&);
    Lock& operator=(const Lock&);
  };

  class Unlock {
  public:
    Unlock()
    {
      _mutex.unlock();
    }
    ~Unlock() {
      _mutex.lock();
    }
  private:
    Unlock(const Unlock&);
    Unlock& operator=(const Unlock&);
  };
  void lock() { _mutex.lock(); }
  void unlock() { _mutex.unlock(); }
  typedef volatile T VolatileType;
private:
  static Mutex _mutex;
};

template <typename T>
Mutex ClassLevelLockable<T>::_mutex;

} // !navi

#endif /* !_LOCK_H_ */
