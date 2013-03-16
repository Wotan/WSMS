#ifndef _FOBSERVER_H_
#define _FOBSERVER_H_

# include "IObserver.hh"

namespace navi {

template <typename FUNCTOR>
class FObserver : public IObserver
{
public:
  FObserver(FUNCTOR const& f) :IObserver(),  _f(f) {}
  FObserver(FObserver const& obs) : IObserver(), _f(obs._f) {}
  void notify() { _f(); }
  FObserver* clone() const {
    return new FObserver(*this);
  }
private:
  FUNCTOR _f;
};

template <typename F>
inline FObserver<F> createFObserver(F f)
{
  return FObserver<F>(f);
}

} // !navi

#endif /* !_FOBSERVER_H_ */
