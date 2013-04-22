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
  _commands["x"] = &Debugger::cmdExamineMem;
  _commands["reg"] = &Debugger::cmdReg;
  _commands["breakpoint"] = &Debugger::cmdBreakpoint;
  _commands["b"] = &Debugger::cmdBreakpoint;
  _commands["run"] = &Debugger::cmdRun;
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
    printf("0x%04x: Cannot access memory at address 0x%x\n", addr, addr);
    return 1;
  }
  if (addr == _cpu->_PC)
    printf("=>0x%04x: ", addr);
  else
    printf("  0x%04x: ", addr);
  return _disasembler.displayInstruction(mem, 5);
}

void Debugger::cmdNext(Command const& cmd)
{
  int nb = 1;
  if (cmd.size() > 1) {
    getAddress(cmd.at(1), nb);
  }
  for (int i = 0; i < nb; ++i)
    _core->step();
  disasInstr(_cpu->PC);
}

UWORD Debugger::getRegisterValue(std::string const& reg)
{
#define STR_REG(v) else if (reg == #v) { return _cpu->v; }

  if (reg == "PC")
    return _cpu->PC;
  STR_REG(AF)
  STR_REG(BC)
  STR_REG(DE)
  STR_REG(HL)
  STR_REG(IX)
  STR_REG(IY)
  STR_REG(PC)
  STR_REG(SP)
  return 0;
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

bool Debugger::getAddress(std::string const& str, int& num)
{
  if (std::strncmp(str.c_str(), "0x", 2) == 0) { // Hexa
      return toBase10(std::string(str.c_str() + 2), num, 16);
  } else if (str[0] == '$') { // register
    num = getRegisterValue(std::string(str.c_str() + 1));
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
  int size = 0x10;
  int address = _cpu->PC;
  if (cmd.size() > 2) {
    getAddress(cmd.at(1), size);
    getAddress(cmd.at(2), address);
  } else if (cmd.size() > 1) {
    getAddress(cmd.at(1), address);
  }
  for (int i = 0; i < size;)
    i += disasInstr(address + i);
}

void Debugger::cmdExamineMem(Command const& cmd)
{
  if (cmd.size() < 2) {
    std::cerr << "Command: " << cmd.at(0) << ": not enough arguments" << std::endl;
    return;
  }
  int size = 1;
  int address = 0;
  if (cmd.size() > 2) {
    getAddress(cmd.at(1), size);
    getAddress(cmd.at(2), address);
  } else {
    getAddress(cmd.at(1), address);
  }
  for (int i = 0; i < size;) {
    printf("%04X: ", address + i);
    for (int j = 0; j < 8 && i < size; ++j, ++i) {
      printf("%02X ", _cpu->READ_MEM(address + i + j));
    }
    printf("\n");
  }
}

void Debugger::printReg(UWORD reg)
{
  printf("%02x %02x (", reg >> 8, reg & 0xFF);
  for (std::size_t i = 0; i < sizeof(reg) * 8; ++i)
    printf("%i", !!(reg & (1 << (sizeof(reg) * 8 - i - 1))));
  printf(")\n");
}

void Debugger::cmdReg(Command const&)
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

void Debugger::cmdBreakpoint(Command const& cmd)
{
  std::string action = "add";
  int addr;
  if (cmd.size() == 1) {
    std::cout << "Breakpoints:" << std::endl;
    int i = 1;
    for (BreakpointSet::iterator it = _breakpoints.begin();
         it != _breakpoints.end(); ++i, ++it) {
      printf("#%i 0x%04x\n", i, *it);
    }
    return;
  } else if (cmd.size() == 2) {
    getAddress(cmd.at(1), addr);
  } else {
    action = cmd.at(1);
    getAddress(cmd.at(2), addr);
  }

  if (action == "add") {
    _breakpoints.insert(addr);
    std::cout << "Breakpoint set at address 0x" << std::hex << addr << std::endl;
  } else if (action == "remove") {
    BreakpointSet::iterator it = _breakpoints.find(addr);
    if (it == _breakpoints.end())
      std::cerr << "Breakpoint 0x" << addr << " doesn't exist" << std::endl;
    else {
      _breakpoints.erase(it);
      std::cout << "Breakpoint removed at address 0x" << std::hex << addr << std::endl;
    }
  } else {
    std::cerr << "Unknow action \"" << cmd.at(1) << "\"" << std::endl;
  }
}

void Debugger::cmdRun(Command const&)
{
  bool run = true;
  while (run) {
    _core->step();
    if (_breakpoints.find(_cpu->PC) != _breakpoints.end()) {
      run = false;
      disasInstr(_cpu->PC);
    }
  }
}

} // !WSMS
