#ifndef _DASM_H_
#define _DASM_H_

# include "navi/Exception.hh"

# include "WSMS.hh"

struct InstrDesc {
  UBYTE opcode1;
  UBYTE opcode2;
  UBYTE opcode3;
  int nbOpcode;
  char const* string;
};

class Disassembler {
  DEFINE_EXCEPTION(Error);
public:
  Disassembler() {}
  virtual ~Disassembler() {}

  int disasInstruction(UBYTE*, int size, std::string &str);
  int displayInstruction(UBYTE* mem, int size);
  InstrDesc findInstruction(UBYTE* mem);

  static bool findPattern(char const* pattern, char const* str);
};

#endif /* !_DASM_H_ */
