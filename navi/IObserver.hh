#ifndef _IOBSERVER_H_
# define _IOBSERVER_H_

namespace navi {

class IObserver {
public:
  virtual ~IObserver() {}
  virtual void notify() = 0;
  virtual IObserver* clone() const = 0;
};

} // !navi

#endif /* !_IOBSERVER_H_ */
