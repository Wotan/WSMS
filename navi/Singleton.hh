#ifndef _SINGLETON_H_
# define _SINGLETON_H_

# include <cstdlib>
# include "CreationPolicies.hh"

namespace navi {

  template <class T>
  struct DestroyAtExit {
    typedef void (*DestroyFun)();
    static void ScheduleDestruction(DestroyFun fun) {
      atexit(fun);
    }
  };

  template <class T>
  struct SingleThreaded
  {
    class Lock {
    public:
      Lock() {}
      ~Lock() {}
    };
    typedef T VolatileType;
  };

  template <
    typename T,
    template <class> class threadingPolicy = SingleThreaded,
    template <class> class creationPolicy = CreateWithNew,
    template <class> class lifetimePolicy = DestroyAtExit
    >
  class SingletonHolder {
  public:
    static T& instance();

  private:
    static void destroy() {
      creationPolicy<T>::destroy(_instance);
      _instance = 0;
    }
    static void createInstance();
    SingletonHolder();
  private:
    typedef typename threadingPolicy<T*>::VolatileType InstanceType;
    static InstanceType _instance;
  };

template <
  typename T,
  template <class> class threadingPolicy,
  template <class> class creationPolicy,
  template <class> class lifetimePolicy
  >
inline T& SingletonHolder<T, threadingPolicy, creationPolicy, lifetimePolicy>::instance()
{
  if (!_instance)
    createInstance();
  return *_instance;
}

template <
  typename T,
  template <class> class threadingPolicy,
  template <class> class creationPolicy,
  template <class> class lifetimePolicy
  >
void SingletonHolder<T, threadingPolicy, creationPolicy, lifetimePolicy>::createInstance()
{
  typename threadingPolicy<SingletonHolder>::Lock guard;
  (void)guard;
  if (!_instance) {
    _instance = creationPolicy<T>::create();
    lifetimePolicy<T>::ScheduleDestruction(&destroy);
  }
}

template <
  class A,
  template <class> class B,
  template <class> class C,
  template <class> class D
  >
typename SingletonHolder<A, B, C, D>::InstanceType
SingletonHolder<A, B, C, D>::_instance = 0;

} // !navi

#endif /* !_SINGLETON_H_ */
