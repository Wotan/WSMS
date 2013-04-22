#ifndef _CORE_H_
# define _CORE_H_

# include "WSMS.hh"
# include "IRenderer.hh"

namespace WSMS {

class VDP;
class MMU;
class Z80;

class Core {
public:
  Core();
  virtual ~Core();

  void init();
  void loadCartridge(std::string const& fileName);
  void run(bool debug);
  void step();

  UBYTE readIOPorts(UBYTE);
  void writeIOPorts(UBYTE, UBYTE);

private:
  IRenderer* _renderer;
  Z80* _cpu;
  MMU* _mmu;
  VDP* _vdp;
  bool _running;
};

}

#endif /* !_CORE_H_ */
