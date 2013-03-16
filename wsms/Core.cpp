#include <iostream>

#include "Core.hh"

namespace WSMS {

Core::Core()
  : _cpu(0), _mmu(0), _running(true)
{
}


Core::~Core()
{
  delete _mmu;
}

void Core::init()
{
  if (!_mmu)
    _mmu = new MMU();
  _cpu = new Z80();
  _cpu->registerMMU(_mmu);
}

void Core::loadCartridge(std::string const& fileName)
{
  assert(_mmu);
  _mmu->loadCartridge(fileName);
}

void Core::run()
{
  uint32_t cycles = 0;

  // while (_running) {
    cycles += _cpu->step();
  // }
}

}
