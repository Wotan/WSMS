#include <iostream>
#include <cstdio>

#include "Core.hh"
#include "Debugger.hh"
#include "CPU.hh"
#include "MMU.hh"
#include "VDP.hh"
#include "SDL/SDLRenderer.hh"

namespace WSMS {

Core::Core()
  : _cpu(0), _mmu(0), _running(true)
{
}


Core::~Core()
{
  delete _mmu;
  delete _cpu;
}

void Core::init()
{
  _renderer = new SDLRenderer();
  _mmu = new MMU();
  _cpu = new Z80();
  _vdp = new VDP();
  _cpu->registerMMU(_mmu);
  _cpu->registerCore(this);
  _renderer->init();
}

void Core::loadCartridge(std::string const& fileName)
{
  assert(_mmu);
  _mmu->loadCartridge(fileName);
}

void Core::run(bool debug)
{
  if (debug) {
    Debugger debugger;
    debugger.setCPU(_cpu);
    debugger.setMMU(_mmu);
    debugger.setCore(this);
    debugger.run();
  } else {
    while (_running)
      step();
  }
}

void Core::step()
{
  uint32_t cycles;
  cycles = _cpu->step();
  if (_vdp->update(cycles)) {
    _renderer->update();
    _renderer->render(_vdp->getScreen());
  }
}

UBYTE Core::readIOPorts(UBYTE a)
{
  if (a < 0x40) {
    return 0xFF; // SMS2 behavior
  } else if (a < 0x80) {
    if ((a % 2) == 0) // even
      return _vdp->getVCounter();
    else // odd
      return _vdp->getHCounter();
  } else if (a < 0xC0) {
    if ((a % 2) == 0) // even
      _vdp->readDataPort();
    else // odd
      _vdp->getStatusFlag();
  } else {
    if ((a % 2) == 0) // even
      ;// Controller A/B
    else // odd
      ;// Controller B/Misc
  }
  return 0;
}

void Core::writeIOPorts(UBYTE a, UBYTE v)
{
  if (a < 0x40) {
    if ((a % 2) == 0) // even
      ; // Memory control reg
    else // odd
      ;// I/O control reg
  } else if (a < 0x80) {
    ;// PSG
  } else if (a < 0xC0) {
    if ((a % 2) == 0) // even
      _vdp->writeDataPort(v);
    else // odd
      _vdp->writeControlPort(v);
  }
  // address 0xC0 to 0xFF have no effect
}

}
