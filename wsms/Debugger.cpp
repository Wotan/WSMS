#include <iostream>
#include <cstdio>

#include "navi/Split.hh"
#include "navi/LexicalCast.hh"

#include "Debugger.hh"
#include "CPU.hh"
#include "MMU.hh"

namespace WSMS {

Debugger::Debugger()
  : _cpu(0), _mmu(0)
{
  _commands["next"] = &Debugger::cmdNext;
  _commands["n"] = &Debugger::cmdNext;
  _commands["xi"] = &Debugger::cmdExamineInstr;
  _commands["reg"] = &Debugger::cmdReg;
  // _commands["breakpoint"] = &Debugger::cmdBreakpoint;
  // _commands["b"] = &Debugger::cmdBreakpoint;
}

bool Debugger::getCommand(std::string& line)
{
  std::cout << "(wsms) ";
  char buffer[512];
  std::cin.getline(buffer, 512);
  line = buffer;
  if (std::cin.eof())
    return false;
  return true;
}

bool Debugger::parseCommand(std::string const& line, Command& cmd)
{
  navi::split(line, cmd);
  return true;
}

void Debugger::executeCommand(Command const& cmd)
{
  CommandMap::iterator callback = _commands.find(cmd.at(0));
  if (callback == _commands.end()) {
    std::cerr << "Undefined command: \"" << cmd.at(0) << "\"." << std::endl;
  } else {
    (this->*callback->second)(cmd);
  }
}

void Debugger::run()
{
  assert(_cpu);
  assert(_mmu);
  bool run = true;
  Command lastCmd;

  while (run) {
    std::string line;
    Command cmd;
    if (!getCommand(line))
      break;
    if (!parseCommand(line, cmd))
      std::cerr << "Error in command parsing" << std::endl;
    if (cmd.size() == 0) {
      if (lastCmd.size() != 0)
        executeCommand(lastCmd);
      continue;
    }
    lastCmd = cmd;
    executeCommand(cmd);
  }
  std::cout << "Exiting..." << std::endl;
}

int Debugger::disasInstr(UWORD addr)
{
  UBYTE mem[5];
  try {
    for (int i = 0; i < 5; ++i)
      mem[i] = READ_MEM(addr + i);
  } catch (std::exception const&) {
    printf("0x%08x: Cannot access memory at address 0x%x\n", addr, addr);
    return 1;
  }
  if (addr == _cpu->_PC)
    printf("=>0x%08x: ", addr);
  else
    printf("  0x%08x: ", addr);
  return _disasembler.displayInstruction(mem, 5);
}

void Debugger::cmdNext(Command const& cmd)
{
  int nb = 1;
  if (cmd.size() > 1) {
    toBase10(cmd.at(1), nb);
  }
  for (int i = 0; i < nb; ++i)
    _cpu->step();
  disasInstr(_cpu->PC);
}

bool Debugger::toBase10(std::string const& str, int& num , int base)
{
  num = 0;
  for (std::size_t i = 0; i < str.size(); ++i)
    {
      int n;
      if (std::isdigit(str[i]))
        n = str[i] - '0';
      else if (str[i] >= 'a' && str[i] <= 'f')
        n = 10 + str[i] - 'a';
      else if (str[i] >= 'A' && str[i] <= 'F')
        n = 10 + str[i] - 'A';
      else
        return false;
      num += n * pow(base, str.size() - (i + 1));
    }
  return true;
}

bool Debugger::toBase10(std::string const& str, int& num)
{
  if (std::strncmp(str.c_str(), "0x", 2) == 0) {
      return toBase10(std::string(str.c_str() + 2), num, 16);
  } else {
    try {
      num = navi::LexicalCast<int>(str);
      return true;
    } catch (std::exception const&) {
      return false;
    }
  }
  return false;
}

void Debugger::cmdExamineInstr(Command const& cmd)
{
  if (cmd.size() < 2) {
    std::cerr << "Command: " << cmd.at(0) << ": not enought arguments" << std::endl;
    return;
  }
  int size = 1;
  int address = 0;
  if (cmd.size() > 2) {
    toBase10(cmd.at(1), size);
    toBase10(cmd.at(2), address);
  } else {
    toBase10(cmd.at(1), address);
  }
  for (int i = 0; i < size;)
    i += disasInstr(address + i);
}

void Debugger::printReg(UWORD reg)
{
  printf("%02x %02x (", reg >> 8, reg & 0xFF);
  for (std::size_t i = 0; i < sizeof(reg) * 8; ++i)
    printf("%i", !!(reg & (1 << (sizeof(reg) * 8 - i - 1))));
  printf(")\n");
}

void Debugger::cmdReg(Command const& cmd)
{
  printf("S Z 5 H 3 P N C\n");
  printf("%i %i %i %i %i %i %i %i\n",
         _cpu->F_S, _cpu->F_Z, _cpu->F_5, _cpu->F_H,
         _cpu->F_3, _cpu->F_PV, _cpu->F_N, _cpu->F_C);
  printf("AF: "); printReg(_cpu->AF);
  printf("BC: "); printReg(_cpu->BC);
  printf("DE: "); printReg(_cpu->DE);
  printf("HL: "); printReg(_cpu->HL);
  printf("PC = 0x%04x\n", _cpu->PC);
  printf("SP = 0x%04x\n", _cpu->SP);
  printf("IX = 0x%04x IY = 0x%04x\n", _cpu->IX, _cpu->IY);
  printf("I  = 0x%02x  R  = 0x%02x\n", _cpu->I, _cpu->R);
}

} // !WSMS
