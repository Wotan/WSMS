#include <iostream>

#include "CPU.hh"

namespace WSMS {

Z80::Z80()
{
  AF = 0;
  BC = 0;
  DE = 0;
  HL = 0;
  _IX = _IY = 0;
  _SP = 0xdff0; // For testing purpose only, user have to set it
  _PC = 0;
  _I = 0;
  _R = 0;
  _sAF = _sBC = _sDE = _sHL = 0;
}

Z80::~Z80()
{
}

/*
   A 7
   B 0
   C 1
   D 2
   E 3
   H 4
   L 5
   HL 6
*/

int Z80::step()
{
  UBYTE opcode = READ_MEM(_PC++);

  switch (opcode) {
    /* ADC */
  case 0x88 + 7: ADD8(A, A, F_C); return 4;
  case 0x88 + 0: ADD8(A, B, F_C); return 4;
  case 0x88 + 1: ADD8(A, C, F_C); return 4;
  case 0x88 + 2: ADD8(A, D, F_C); return 4;
  case 0x88 + 3: ADD8(A, E, F_C); return 4;
  case 0x88 + 4: ADD8(A, H, F_C); return 4;
  case 0x88 + 5: ADD8(A, L, F_C); return 4;
  case 0xCE: ADD8(A, READ_MEM(PC++), F_C); return 7;
  case 0x8E: ADD8(A, READ_MEM(HL), F_C); return 7;

    /* ADD */
  case 0x80 + 7: ADD8(A, A); return 4;
  case 0x80 + 0: ADD8(A, B); return 4;
  case 0x80 + 1: ADD8(A, C); return 4;
  case 0x80 + 2: ADD8(A, D); return 4;
  case 0x80 + 3: ADD8(A, E); return 4;
  case 0x80 + 4: ADD8(A, H); return 4;
  case 0x80 + 5: ADD8(A, L); return 4;
  case 0xC6: ADD8(A, READ_MEM(PC++)); return 7;
  case 0x86: ADD8(A, READ_MEM(HL)); return 7;
  case 0x09: ADD16(HL, BC); return 11;
  case 0x19: ADD16(HL, DE); return 11;
  case 0x29: ADD16(HL, HL); return 11;
  case 0x39: ADD16(HL, SP); return 11;

    /* AND */
  case 0xA0 + 7: AND(A, A); return 4;
  case 0xA0 + 0: AND(A, B); return 4;
  case 0xA0 + 1: AND(A, C); return 4;
  case 0xA0 + 2: AND(A, D); return 4;
  case 0xA0 + 3: AND(A, E); return 4;
  case 0xA0 + 4: AND(A, H); return 4;
  case 0xA0 + 5: AND(A, L); return 4;
  case 0xE6: AND(A, READ_MEM(PC++)); return 7;
  case 0xA6: AND(A, READ_MEM(HL)); return 7;


  default:
    std::cout << "Unknown opcode: " << std::hex
	      << static_cast<int>(opcode)
	      << std::endl;
    break;
  }
  return 0;
}

void Z80::push8(UBYTE value)
{
  WRITE_MEM(--SP, value);
}

void Z80::push16(UWORD value)
{
  push8(value & 0xff);
  push8(value >> 8);
}

UBYTE Z80::pop8()
{
  return READ_MEM(SP++);
}

UWORD Z80::pop16()
{
  UWORD ret = pop8();
  ret = ret | (pop8() << 8);
  return ret;
}

/* SZHPNC -> ***V0* */
/* NB: use for both ADC 8-bit and ADD 8-bit */
void Z80::ADD8(UBYTE& dest, UBYTE value, UBYTE carry)
{
  F_H = ((dest & 0xf) + ((value + carry) & 0xf)) & 0x10;
  F_C = dest + value + carry > 0xff;
  int32_t calc32u = static_cast<BYTE>(dest) + static_cast<BYTE>(value) + carry;
  F_PV = (calc32u > 127 || calc32u < -128);
  dest = dest + value + carry;
  F_S = dest & 0x80; // negative
  F_Z = (dest == 0);
  F_N = 0;
}

/* SZHPNC -> --?-0* */
void Z80::ADD16(UWORD& dest, UWORD value)
{
  F_H = ((dest & 0xf) + ((value + F_C) & 0xf)) & 0x10;
  F_C = 0;
  dest = dest + value + F_C;
}

/* SZHPNC -> **?V0* */
void Z80::ADC16(UWORD& dest, UWORD value)
{
  // assume in 16bit we take only 4 lsb
  F_H = ((dest & 0xf) + ((value + F_C) & 0xf)) & 0x10;
  F_C = dest + value + F_C > 0xffff;
  int32_t calc32u = static_cast<BYTE>(dest) + static_cast<BYTE>(value) + F_C;
  F_PV = (calc32u > 32767 || calc32u < -32768);
  dest = dest + value + F_C;
  F_S = dest & 0x8000;
  F_Z = (dest == 0);
  F_N = 0;
}

/* SZHPNC -> ***P00 */
void Z80::AND(UBYTE& dest, UBYTE value)
{
  // F_H : right ?
  F_H = ((dest & 0xf) + (value & 0xf)) & 0x10;
  dest = dest & value;
  F_S = dest & 0x80;
  F_Z = (dest == 0);
  F_PV = (dest % 2 || dest == 1);
  F_N = F_C = 0;
}

/* SZHPNC -> ?*1?0- */
void Z80::BIT(UBYTE value, UBYTE bit)
{
  F_Z = value & (1 << bit);
  F_H = 1;
  F_N = 0;
}

void Z80::CALL(UBYTE value)
{
}


}
