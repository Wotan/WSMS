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
# define B _AF.hi
# define C _AF.lo
# define D _AF.hi
# define E _AF.lo
# define H _AF.hi
# define L _AF.lo

# define AF _AF.a
# define BC _BC.a
# define DE _DE.a
# define HL _HL.a

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

# define READ_MEM(a) (_mmu->readMem(a))
# define WRITE_MEM(a, d) (_mmu->writeMem(a, d))

# define READ_MEM16(a) (_mmu->readMem16(a))
# define WRITE_MEM16(a, d) (_mmu->writeMem16(a, d))

# define READ_IO(a) (_mmu->readIOPorts(a))
# define WRITE_IO(a, d) (_mmu->writeIOPorts(a, d))

class Z80 {
public:
  Z80();
  virtual ~Z80();
  void registerMMU (MMU* mmu) {
    _mmu = mmu;
  }
  int step();

  void push8(UBYTE value);
  void push16(UWORD value);
  UBYTE pop8();
  UWORD pop16();

  void ADD8(UBYTE& dest, UBYTE value, UBYTE carry = 0);
  void ADD16(UWORD& dest, UWORD value);
  void ADC16(UWORD& dest, UWORD value);
  void AND(UBYTE& dest, UBYTE value);
  void BIT(UBYTE value, UBYTE bit);
  bool CALL(bool cond);

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
};

} // !WSMS

#endif /* !_CPU_H_ */
