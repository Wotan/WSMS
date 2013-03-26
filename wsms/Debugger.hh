#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

# include <map>
# include <vector>

# include "WSMS.hh"
# include "Disassembler.hh"

namespace WSMS {

class Z80;
class MMU;

class Debugger {
  typedef std::vector<std::string> Command;
  typedef void (Debugger::*CommandCallback)(Command const&);
  typedef std::map<std::string, CommandCallback> CommandMap;
public:
  Debugger();
  virtual ~Debugger() {}

  void setCPU(Z80 *cpu) { _cpu = cpu; }
  void setMMU(MMU *mmu) { _mmu = mmu; }

  bool getCommand(std::string& line);
  bool parseCommand(std::string const& line, Command& cmd);
  void executeCommand(Command const& cmd);
  void run();

  bool toBase10(std::string const& str, int& num);
  bool toBase10(std::string const& str, int& num , int base);

  void printReg(UWORD reg);

  void cmdNext(Command const&);
  void cmdExamineInstr(Command const&);
  int disasInstr(UWORD addr);

  void cmdReg(Command const& cmd);

private:
  Z80* _cpu;
  MMU* _mmu;
  CommandMap _commands;
  Disassembler _disasembler;
};

} // !WSMS

#endif /* !_DEBUGGER_H_ */
