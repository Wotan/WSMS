#ifndef _CPU_H_
# define _CPU_H_

# include "WSMS.hh"
# include "MMU.hh"

namespace WSMS {

typedef union
{
  struct
  {
    UBYTE lo;
    UBYTE hi;
  };
  UWORD  a;
} DRegister;

typedef union
{
  struct
  {
    union
    {
      UBYTE lo;
      struct
      {
        bool c : 1;
        bool n : 1;
        bool pv : 1;
        bool _3 : 1;
        bool h : 1;
        bool _5: 1;
        bool z : 1;
        bool s : 1;
      };
    };
    UBYTE hi;
  };
  UWORD  a;
} FRegister;

# define A _AF.hi
# define F _AF.lo
# define B _BC.hi
# define C _BC.lo
# define D _DE.hi
# define E _DE.lo
# define H _HL.hi
# define L _HL.lo

# define I _I
# define R _R

# define AF _AF.a
# define BC _BC.a
# define DE _DE.a
# define HL _HL.a

# define IX _IX
# define IY _IY

# define PC _PC
# define SP _SP

# define F_C _AF.s
# define F_N _AF.n
# define F_PV _AF.pv
# define F_3 _AF._3
# define F_H _AF.h
# define F_5 _AF._5
# define F_Z _AF.z
# define F_S _AF.s

# define READ_MEM(a) _mmu->readMem(a)
# define WRITE_MEM(a, d) _mmu->writeMem(a, d)

# define READ_MEM16(a) _mmu->readMem16(a)
# define WRITE_MEM16(a, d) _mmu->writeMem16(a, d)

# define READ_IO(a) _core->readIOPorts(a)
# define WRITE_IO(a, d) _core->writeIOPorts(a, d)

# define UNKNOW_EXT_OPCODE(op) \
  std::cout << "Unknown extended opcode (" << op << "): " << std::hex \
     << static_cast<int>(opcode)     \
     << std::endl;                   \

# define EXT_OPCODE(op) case (op): { UBYTE opcode = READ_MEM(PC++); switch(opcode) {
# define EXT_OPCODE_END(op) default: UNKNOW_EXT_OPCODE(op) return 0; } } break;

# define UNSUPPORTED_INSTR std::cout << "Unsupported instr" << std::endl;

typedef enum {
  INT_MODE_0,
  INT_MODE_1,
  INT_MODE_2
} interuptMode;

class Debugger;
class Core;

class Z80 {
  friend class Debugger;
public:
  Z80();
  virtual ~Z80();

  void registerCore(Core* core) { _core = core; }
  void registerMMU (MMU* mmu) { _mmu = mmu; }
  int step();

  UBYTE indexInstructions(UWORD& X, UBYTE opcode);
  void setIntMode(interuptMode intMode) { _intMode = intMode; }
  void setIntEnable(bool e = true) { _intEnabled = e; }

  void push8(UBYTE value);
  void push16(UWORD value);
  UBYTE pop8();
  UWORD pop16();

  void ADD8(UBYTE& a, UBYTE b, UBYTE carry = 0);
  void ADD16(UWORD& a, UWORD b);
  void ADC16(UWORD& a, UWORD b);

  void SUB8(UBYTE& a, UBYTE b, UBYTE carry);
  void SBC16(UWORD& a, UWORD b);

  void AND(UBYTE& a, UBYTE b);
  void OR(UBYTE& a, UBYTE b);
  void XOR(UBYTE& a, UBYTE b);

  void RL(UBYTE& a);
  void RLC(UBYTE& a);
  void RR(UBYTE& a);
  void RRC(UBYTE& a);

  void SLA(UBYTE& a);
  void SRA(UBYTE& a);
  void SLL(UBYTE& a);
  void SRL(UBYTE& a);

  void BIT(UBYTE value, UBYTE bit);
  void RES(UBYTE& v, UBYTE bit);
  void SET(UBYTE& v, UBYTE bit);

  bool CALL(bool cond);

  void CP(UBYTE a, UBYTE b);
  void CPD();
  BYTE CPDR();
  void CPI();
  BYTE CPIR();

  void DEC8(UBYTE& a);
  void DEC16(UWORD& a);
  void INC8(UBYTE& a);
  void INC16(UWORD& a);
  void EX(UWORD& a, UWORD& b);

  UBYTE IN(UBYTE a);
  void OUT(UBYTE a);

  void NEG(UBYTE& a);

private:
  FRegister _AF;
  DRegister _BC;
  DRegister _DE;
  DRegister _HL;
  UWORD _IX;
  UWORD _IY;
  UWORD _SP;
  UWORD _PC;
  UBYTE _I;
  UBYTE _R;

  // Shadow
  UWORD _sAF;
  UWORD _sBC;
  UWORD _sDE;
  UWORD _sHL;

  MMU* _mmu;
  Core* _core;

  bool _intEnabled;
  interuptMode _intMode;
};

} // !WSMS

#endif /* !_CPU_H_ */
