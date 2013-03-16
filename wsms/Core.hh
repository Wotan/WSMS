#ifndef _CORE_H_
# define _CORE_H_

# include "CPU.hh"
# include "MMU.hh"

namespace WSMS {

class Core {
public:
  Core();
  virtual ~Core();

  void init();
  void loadCartridge(std::string const& fileName);
  void run();

private:
  Z80* _cpu;
  MMU* _mmu;
  bool _running;
};

}

#endif /* !_CORE_H_ */
